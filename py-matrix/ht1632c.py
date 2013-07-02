#----------------------------------------------------------------------
# The datasheet for the HT1632C is available
#   http://www.adafruit.com/datasheets/ht1632cv120.pdf
#
# WiringPi pinout reference
#   https://projects.drogon.net/raspberry-pi/wiringpi/pins/
#
# Some important things to know about this device and SPI:
#
# - The SPI interface has no MISO connection.  It is write-only.
#
# - The spidev xfer and xfer2 calls overwrite the output buffer
#   with the bytes read back in during the SPI transfer.
#   Use writebytes instead of xfer to avoid having your buffer overwritten.
#
# - The D/C (Data/Command) line is used to distinguish data writes
#   and command writes - HIGH for data, LOW for commands.  To be clear,
#   the attribute bytes following a command opcode are NOT considered data,
#   data in this case refers only to the display memory buffer.
#   keep D/C LOW for the command byte including any following argument bytes.
#   Pull D/C HIGH only when writting to the display memory buffer.
#  
# - The pin connections between the Raspberry Pi and OLED module are:
#
#      RPi      HTC1632C
#      GPIO23 -> CS1
#      GPIO24 -> CS2
#      GPIO25 -> CS3
#      GPIO26 -> CS4
#      SCLK   -> CLK
#      MOSI   -> DATA
#----------------------------------------------------------------------

import spidev
import wiringpi2 as wiringpi
import time
import font5x8
#import sys
#import os

class HT1632C:

    # Class constants are externally accessible as gaugette.ssd1306.SSD1306.CONST
    # or my_instance.CONST
    #SPI_SPEED        = 2000000
    SPI_SPEED        = 1000000

    CMD_COMMAND_ID   = 0x4
    CMD_WRITE_ID     = 0x5
    CMD_SYS_DIS      = 0x00
    CMD_SYS_EN       = 0x01
    CMD_LED_OFF      = 0x02
    CMD_LED_ON       = 0x03
    CMD_PWM_CONTROL  = 0xA0
    CMD_BLINK_OFF    = 0x08
    CMD_BLINK_ON     = 0x09
    CMD_SLAVE_MODE   = 0x10
    CMD_MASTER_MODE  = 0x18
    CMD_COMMON_8NMOS = 0x20
    CMD_COMMON_8PMOS = 0x28

    BLINK_ON  = 1
    BLINK_OFF = 0
    DELAY     = 550 # ms

    CS0 = 3 # GPIO 22
    CS1 = 4 # GPIO 23
    CS2 = 5 # GPIO 24
    CS3 = 6 # GPIO 25 A activer en continu avec le 74HC138 (pin E3)

    # Device name will be /dev/spidev-{bus}.{device}
    # dc_pin is the data/commmand pin.  This line is HIGH for data, LOW for command.
    # We will keep d/c low and bump it high only for commands with data
    # reset is normally HIGH, and pulled LOW to reset the display

    def __init__(self, bus=0, device=0, buffer_rows=8, buffer_cols=32, rows=8, cols=32, nbMatrix=1):
        self.nbMatrix = nbMatrix
        self.cols = cols * nbMatrix
        self.cols_per_matrix = 32
        self.rows = rows
        self.buffer_rows = buffer_rows
        self.buffer_cols = buffer_cols
        self.mem_bytes = self.buffer_cols * self.buffer_rows / 8 # total bytes in HTC1632C display ram

        self.spi = spidev.SpiDev()
        self.spi.open(bus, device)
        self.spi.max_speed_hz = self.SPI_SPEED
        self.spi.bits_per_word = 8

        self.gpio = wiringpi.GPIO(wiringpi.GPIO.WPI_MODE_PINS)
        self.gpio.pinMode(self.CS0, self.gpio.OUTPUT)
        self.gpio.pinMode(self.CS1, self.gpio.OUTPUT)
        self.gpio.pinMode(self.CS2, self.gpio.OUTPUT)
        self.gpio.pinMode(self.CS3, self.gpio.OUTPUT)
        self.gpio.digitalWrite(self.CS3, self.gpio.HIGH)
        self.font = font5x8.Font5x8
        self.col_offset = 0
        self.bitmap = self.Bitmap(buffer_cols, buffer_rows)

    def reset(self):
        data = [0] * (self.cols / self.nbMatrix)
        for m in range(self.cols / self.nbMatrix):
            data[m] = 0x00
        for i in range(self.nbMatrix):
            self.write2(i, data)

    def full(self):
        data = [0] * (self.cols / self.nbMatrix)
        for m in range(self.cols / self.nbMatrix):
            data[m] = 0xff
        for i in range(self.nbMatrix):
            self.write2(i, data)

    def alternate(self):
        data = [0] * (self.cols / self.nbMatrix)
        for m in range(self.cols / self.nbMatrix):
            data[m] = 0xff if m%2 == 0 else 0x00
        for i in range(self.nbMatrix):
            self.write2(i, data)

    def command(self, chip, bytes):
        # already low
        self.chipSelect(chip)
        print "bytes 0x%02X" % bytes
        data = [(0x80 | bytes >> 4), ((bytes << 4) & 0xff)]
        print hex(data[0])
        print hex(data[1])
        print self.spi.writebytes(data)
        #self.gpio.delay(self.DELAY)
        self.gpio.delayMicroseconds(self.DELAY)
        self.chipSelect(99)

    def reverseByteOrder(self, data):
        "Reverses the byte order of an int (16-bit) or long (32-bit) value"
        # Courtesy Vishal Sapre
        byteCount = len(hex(data)[2:].replace('L','')[::2])
        val       = 0
        for i in range(byteCount):
            val    = (val << 8) | (data & 0xff)
            data >>= 8
        return val

    def write2(self, chip, bytes):
        self.chipSelect(chip)
        self.gpio.delayMicroseconds(self.DELAY)
        length = len(bytes)
        start = 0
        data = [0] * (length + 2)
        data[0] = 0xA0
        data[1] = (0x00 | ((bytes[0] >> 2) & 0xff))
        print "First data: 0x%02X" % data[0]
        print "Second data: 0x%02X" % data[1]
        j = 2
        for i in range(start, length):
            if i >= length-1:
                nextByte = 0x00
            else:
                nextByte = ((bytes[i+1] >> 2) & 0xff)
            data[j] = (((bytes[i] << 6) & 0xff) | nextByte)
            print "0x%02X" % data[j]
            j += 1
        self.spi.writebytes(data)
        self.gpio.delayMicroseconds(self.DELAY)
        #self.gpio.delay(self.DELAY)
        self.chipSelect(99)

    def writeMatrix(self, bytes):
        length = len(bytes)
        for i in range(length/32):
            self.write2(i, bytes[i*32:(i*32)+32])

    def write(self, bytes):
        print "length data: %d" % len(bytes)
        for m in range(self.nbMatrix):
            self.chipSelect(m)
            length = ((m*32)+32)
            start = (m*32)
            data = [0] * (length + 2)
            data[0] = 0xA0
            data[1] = 0x00 | ((bytes[start] >> 2) & 0xff)
            print "length: %d" % length
            print "First data: %s" % hex(data[0])
            print "Second data: %s" % hex(data[1])
            print "Start %d" % start
            j=2
            for i in range(start, length):
                if i >= length-1:
                    nextByte = 0x3f
                else:
                    nextByte = ((bytes[i+1] >> 2) & 0xff)
                data[j] = ((bytes[i] << 6) & 0xff) | nextByte
                #data[j] = bytes[i]
                print "0x%02X" % data[j]
                j += 1
            self.spi.writebytes(data)
            self.gpio.delayMicroseconds(self.DELAY)
            #time.sleep(0.002)
            self.chipSelect(-1)

    def brightness(self, chip, pwm):
        if pwm > 15:
            pwm = 15
        self.command(chip, self.CMD_PWM_CONTROL | pwm);

    def blink(self, chip, blinky):
        if blinky:
            self.command(chip, self.CMD_BLINK_ON)
        else:
            self.command(chip, self.CMD_BLINK_OFF)

    def chipSelect(self, id):
        if id == 0:
            print "chip: %d" % id
            self.gpio.digitalWrite(self.CS0, self.gpio.LOW)
            self.gpio.digitalWrite(self.CS1, self.gpio.LOW)
            self.gpio.digitalWrite(self.CS2, self.gpio.LOW)
        elif id == 1:
            print "chip: %d" % id
            self.gpio.digitalWrite(self.CS0, self.gpio.LOW)
            self.gpio.digitalWrite(self.CS1, self.gpio.LOW)
            self.gpio.digitalWrite(self.CS2, self.gpio.HIGH)
        elif id == 2:
            print "chip: %d" % id
            self.gpio.digitalWrite(self.CS0, self.gpio.LOW)
            self.gpio.digitalWrite(self.CS1, self.gpio.HIGH)
            self.gpio.digitalWrite(self.CS2, self.gpio.LOW)
        elif id == 3:
            print "chip: %d" % id
            self.gpio.digitalWrite(self.CS0, self.gpio.LOW)
            self.gpio.digitalWrite(self.CS1, self.gpio.HIGH)
            self.gpio.digitalWrite(self.CS2, self.gpio.HIGH)
        else:
            print "chip: %d" % id
            self.gpio.digitalWrite(self.CS0, self.gpio.HIGH)
            self.gpio.digitalWrite(self.CS1, self.gpio.HIGH)
            self.gpio.digitalWrite(self.CS2, self.gpio.HIGH)
        #self.gpio.delay(self.DELAY)
        self.gpio.delayMicroseconds(self.DELAY)

    def begin(self):
        for i in range(self.nbMatrix):
            self.command(i, self.CMD_MASTER_MODE)
            self.command(i, self.CMD_SYS_EN)
            self.command(i, self.CMD_LED_ON)
            self.brightness(i, 15)
            self.blink(i, self.BLINK_OFF)
        #self.reset()

    def display(self):
        self.display_block(self.bitmap, 0, 0, self.cols, self.col_offset)

    def display_block(self, bitmap, row, col, col_count, col_offset=0):
        page_count = bitmap.rows >> 3
        page_start = row >> 3
        page_end   = page_start + page_count - 1
        col_start  = col
        col_end    = col + col_count - 1
        start = col_offset * page_count
        length = col_count * page_count
        self.writeMatrix(bitmap.data[start:start+length])

    # Diagnostic print of the memory buffer to stdout 
    def dump_buffer(self):
        self.bitmap.dump()

    def draw_pixel(self, x, y, on=True):
        self.bitmap.draw_pixel(x,y,on)

    def draw_text(self, x, y, string):
        font_bytes = self.font.bytes
        font_rows = self.font.rows
        font_cols = self.font.cols
        for c in string:
            p = ord(c) * font_cols
            for col in range(0, font_cols):
                mask = font_bytes[p]
                p+=1
                for row in range(0,8):
                    self.draw_pixel(x,y+row,mask & 0x1)
                    mask >>= 1
                x += 1

    def draw_text2(self, x, y, string, size=2, space=1):
        font_bytes = self.font.bytes
        font_rows = self.font.rows
        font_cols = self.font.cols
        for c in string:
            p = ord(c) * font_cols
            for col in range(0, font_cols):
                mask = font_bytes[p]
                p+=1
                py = y
                for row in range(0, 8):
                    for sy in range(0, size):
                        px = x
                        for sx in range(0, size):
                            self.draw_pixel(px, py, mask & 0x1)
                            px += 1
                        py += 1
                    mask >>= 1
                x += size
            x += space

    def draw_text3(self, x, y, string):
        return self.bitmap.draw_text(x, y, string, self.font)


    class Bitmap:

        def __init__(self, cols, rows):
            self.rows = rows
            self.cols = cols
            self.bytes_per_col = rows / 8
            self.data = [0] * (self.cols * self.bytes_per_col)

        def clear(self):
            for i in range(0,len(self.data)):
                self.data[i] = 0

        # Diagnostic print of the memory buffer to stdout
        def dump(self):
            for y in range(0, self.rows):
                mem_row = y/8
                bit_mask = 1 << (y % 8)
                line = ''
                for x in range(0, self.cols):
                    mem_col = x
                    offset = mem_row + self.rows/8 * mem_col
                    if self.data[offset] & bit_mask:
                        line += '*'
                    else:
                        line += ' '
                print('|'+line+'|')

        def draw_pixel(self, x, y, on=True):
            if (x<0 or x>=self.cols or y<0 or y>=self.rows):
                return
            mem_col = x
            mem_row = y / 8
            bit_mask = 1 << (y % 8)
            offset = mem_row + self.rows/8 * mem_col
            if on:
                self.data[offset] |= bit_mask
            else:
                self.data[offset] &= (0xFF - bit_mask)

        def clear_block(self, x0, y0, dx, dy):
            for x in range(x0, x0 + dx):
                for y in range(y0, y0 + dy):
                    self.draw_pixel(x,y,0)

        def draw_text(self, x, y, string, font):
            height = font.char_height
            prev_char = None

            for c in string:
                if (c < font.start_char or c > font.end_char):
                    if prev_char != None:
                        x += font.space_width + prev_width + font.gap_width
                    prev_char = None
                else:
                    pos = ord(c) - ord(font.start_char)
                    (width,offset) = font.descriptors[pos]
                    if prev_char != None:
                        x += font.kerning[prev_char][pos] + font.gap_width

                    prev_char = pos
                    prev_width = width

                    bytes_per_row = (width + 7) / 8
                    for row in range(0,height):
                        py = y + row
                        mask = 0x80
                        p = offset
                        for col in range(0,width):
                            px = x + col
                            if (font.bitmaps[p] & mask):
                                self.draw_pixel(px,py,1)  # for kerning, never draw black
                            mask >>= 1
                            if mask == 0:
                                mask = 0x80
                                p+=1
                        offset += bytes_per_row
            if prev_char != None:
                x += prev_width

            return x

    # This is a helper class to display a scrollable list of text lines.
    # The list must have at least 1 item.
    class ScrollingList:
        def __init__(self, ht1632c, list, font):
            self.ht1632c = ht1632c
            self.list = list
            self.font = font
            self.position = 0 # row index into list, 0 to len(list) * self.rows - 1
            self.offset = 0   # led hardware scroll offset
            self.pan_row = -1
            self.pan_offset = 0
            self.pan_direction = 1
            self.bitmaps = []
            self.rows = ht1632c.rows
            self.cols = ht1632c.cols
            self.bufrows = self.rows * 2
            downset = (self.rows - font.char_height) / 2
            for text in list:
                width = ht1632c.cols
                text_bitmap = ht1632c.Bitmap(width, self.rows)
                width = text_bitmap.draw_text(0, downset, text, font)
                if width > 128:
                    text_bitmap = ht1632c.Bitmap(width+15, self.rows)
                    text_bitmap.draw_text(0,downset,text,font)
                self.bitmaps.append(text_bitmap)

            # display the first word in the first position
            self.ht1632c.display_block(self.bitmaps[0], 0, 0, self.cols)

        # how many steps to the nearest home position
        def align_offset(self):
            pos = self.position % self.rows
            midway = (self.rows / 2)
            delta = (pos + midway) % self.rows - midway
            return -delta

        def align(self, delay=0.005):
            delta = self.align_offset()
            if delta != 0:
                steps = abs(delta)
                sign = delta/steps
                for i in range(0,steps):
                    if i>0 and delay>0:
                        time.sleep(delay)
                    self.scroll(sign)
            return self.position / self.rows

        # scroll up or down.  Does multiple one-pixel scrolls if delta is not >1 or <-1
        def scroll(self, delta):
            if delta == 0:
                return

            count = len(self.list)
            step = cmp(delta, 0)
            for i in range(0,delta, step):
                if (self.position % self.rows) == 0:
                    n = self.position / self.rows
                    # at even boundary, need to update hidden row
                    m = (n + step + count) % count
                    row = (self.offset + self.rows) % self.bufrows
                    self.ht1632c.display_block(self.bitmaps[m], row, 0, self.cols)
                    if m == self.pan_row:
                        self.pan_offset = 0
                self.offset = (self.offset + self.bufrows + step) % self.bufrows
                self.ht1632c.command(self.ht1632c.SET_START_LINE | self.offset)
                max_position = count * self.rows
                self.position = (self.position + max_position + step) % max_position

        # pans the current row back and forth repeatedly.
        # Note that this currently only works if we are at a home position.
        def auto_pan(self):
            n = self.position / self.rows
            if n != self.pan_row:
                self.pan_row = n
                self.pan_offset = 0

            text_bitmap = self.bitmaps[n]
            if text_bitmap.cols > self.cols:
                row = self.offset # this only works if we are at a home position
                if self.pan_direction > 0:
                    if self.pan_offset <= (text_bitmap.cols - self.cols):
                        self.pan_offset += 1
                    else:
                        self.pan_direction = -1
                else:
                    if self.pan_offset > 0:
                        self.pan_offset -= 1
                    else:
                        self.pan_direction = 1
                self.ht1632c.display_block(text_bitmap, row, 0, self.cols, self.pan_offset)


# vim: set ai expandtab nu ts=4 sw=4:
