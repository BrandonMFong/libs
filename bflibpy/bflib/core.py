# author: Brando
# date: 6/2/23

def abserr(experimental, actual):
    return abs(experimental - actual)

def relerr(experimental, actual):
    return abserr(experimental, actual) / actual

def percerr(experimental, actual):
    return relerr(experimental, actual) * 100

