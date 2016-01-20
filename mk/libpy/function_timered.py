#coding: utf8
import time
def timered(f):
    def tmp(*args, **kwargs):
        t = time.time()
        res = f(*args, **kwargs)
        print ("Время выполнения функции %s: %f" % (f.__name__, (time.time()-t)))
        return res

    return tmp
    



if __name__ == '__main__':
	
	@timered
	def func(x, y):
		return x + y

	@timered
	def func2(x, y):
		return x * y

	print(func(2,3))
	print(func2(2,3))