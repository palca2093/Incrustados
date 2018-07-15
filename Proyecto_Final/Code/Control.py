import gpiozero


button1 = Button(5)
button2 = Button(6)
button3 = Button(13)
button4 = Button(19)


while True:
    if button1.is_pressed:
        print("Button is pressed")
    else:
        print("Button is not pressed")
