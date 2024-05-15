# author: Brando
# date: 6/2/23

def readfile(filepath: str) -> str:
    with open("filename.txt", "r") as f:
        return f.read()

