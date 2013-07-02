import ht1632c
import time
import sys

led = ht1632c.HT1632C(nbMatrix=4)
led.begin()
led.reset()
#led.full()
#text = time.strftime("%A")
#led.draw_text(5,5,'Test')
#led.display()
