import string

class TextFormat:
    def __init__(self, text):
        for p in ['.',',','!','?']:
            text = text.replace(p,'')
        self.fmtText = text.lower()

    def strLength(self):
        return len(self.fmtText)
    
txt = TextFormat("Hello, World! How. are you mate? Okay")
print(txt.fmtText)
print(txt.strLength())