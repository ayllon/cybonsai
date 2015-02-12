#!/usr/bin/env python
import datetime
import logging
import optparse
import serial
import signal
import sqlite3
import sys
import threading
import time

log = logging.getLogger(__name__)


class Acquirer(threading.Thread):
    
    def __init__(self, device, callback=None):
        super(Acquirer, self).__init__()
        self.device = device
        self.callback = callback
        self.keep_going = True
        self.values = dict()
        
    def _process_data(self, line):
        components = line.split()
        if len(components) < 2:
            return
        label = components[1]
        value = None
        for c in components[2:]:
            try:
                key, val = c.split("=")
                if key == "value":
                    value = float(val)
            except ValueError:
                log.warn("Invalid value: %s" % c)
        if value:
            self.values[label] = value
        if value and self.callback:
           self.callback(label, value)
    
    def run(self):
        server = serial.Serial(self.device, 9600)
        try:
            while self.keep_going:
                line = server.readline().strip()
                if line.startswith("DATA"):
                    self._process_data(line)
                log.debug(line)
        finally:
            server.close()
            
    def stop(self):
        self.keep_going = False


class BaseStore(threading.Thread):
    
    def __init__(self, acquirer, interval):
        super(BaseStore, self).__init__()
        self.acquirer = acquirer
        self.keep_going = True
        self.interval = interval
        
    def setup(self):
        pass
    
    def tear_down(self):
        pass
        
    def store(self, timestamp, key, value):
        pass
    
    def run(self):
        self.setup()
        try:
            while self.keep_going and self.acquirer.is_alive():
                time.sleep(self.interval)
                timestamp = datetime.datetime.utcnow()
                for key, value in self.acquirer.values.iteritems():
                    log.info("Store %s" % key)
                    self.store(timestamp, key, value)
        finally:
            self.tear_down()
    
    def stop(self):
        self.keep_going = False
        
        
class CSVStore(BaseStore):
    
    def __init__(self, path, acquirer, interval):
        super(CSVStore, self).__init__(acquirer, interval)
        self.path = path
        self.fd = open(self.path, "a")
    
    def store(self, timestamp, key, value):
        print >>self.fd, "%s,%s,%s" % (timestamp, key, value)
        self.fd.flush()


class SQLiteStore(BaseStore):
    
    def __init__(self, path, acquirer, interval):
        super(SQLiteStore, self).__init__(acquirer, interval)
        self.path = path
        self.conn = None
        
    def create_tables(self):
        self.conn.execute(
          """CREATE TABLE IF NOT EXISTS t_events
             (timestamp TEXT, sensor TEXT, value REAL)
          """
        )
        
    def setup(self):
        self.conn = sqlite3.connect(self.path)
        self.create_tables()
        
    def store(self, timestamp, key, value):
        self.conn.execute(
          """INSERT INTO t_events VALUES
             (?, ?, ?)
          """,
          (timestamp.strftime("%Y-%m-%dT%H:%M:%S"), key, value)
        )
        self.conn.commit()


if __name__ == "__main__":
    parser = optparse.OptionParser()
    parser.add_option(
        "-i", "--interval", type="int", default=60,
        help="Capture rate"
    )
    parser.add_option(
        "-v", "--verbose", default=False, action="store_true",
        help="Enable verbose mode"
    )
    parser.add_option(
        "--csv", default=False, action="store_true",
        help="Store data as CSV"
    )
    
    opt, args = parser.parse_args()
    
    if len(args) < 2:
        parser.error("Missing serial device")
    if len(sys.argv) < 3:
        parser.error("Missing csv file")
        
    # Setup logging
    handler = logging.StreamHandler(sys.stderr)
    log.addHandler(handler)
    if opt.verbose:
        log.setLevel(logging.DEBUG)
        handler.setLevel(logging.DEBUG)
        
    # Create workers
    acq = Acquirer(args[0])
    if opt.csv:
        store = CSVStore(args[1], acq, opt.interval)
    else:
        store = SQLiteStore(args[1], acq, opt.interval)
    
    # Install signal handler
    def sigint_handler(signum, frame):
        log.warn("Stopping...")
        acq.stop()
        store.stop()
    signal.signal(signal.SIGINT, sigint_handler)
    
    # Start working
    acq.start()
    store.start()

    # Wait for the workers
    while acq.is_alive() and store.is_alive():
        signal.pause()

    sys.exit(0)