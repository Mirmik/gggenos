import threading

class Thread(threading.Thread):
    def __init__(self, f):
        threading.Thread.__init__(self)
        self.run = f

@Thread
def ttt():
    print "This is a thread function"

ttt.start()