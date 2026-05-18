# \# Automatic Street Light Controller

# 

# An Arduino-based automatic street light controller that optimizes energy 

# usage by responding to ambient light levels. Designed to solve the problems 

# of manual operation, fixed thresholds, and flickering in conventional systems.

# 

# \## Features

# \- Automatic ON/OFF based on ambient light (LDR sensor)

# \- Adjustable switching threshold via potentiometer

# \- Hysteresis (dead band) to prevent flickering

# \- 3-second delay before switching to ignore transient shadows

# \- Manual override (AUTO / FORCE-ON / FORCE-OFF) via push button

# \- Status LEDs for mode indication

# 

# \## Components

# \- Arduino Uno

# \- LDR + 10kΩ resistor (voltage divider)

# \- 10kΩ potentiometer (threshold adjustment)

# \- BC547 NPN transistor (relay driver)

# \- 12V relay module

# \- 1N4007 flyback diode

# \- Push button

# \- Green and Red LEDs + 330Ω resistors

# 

# \## Files

# \- `street-light.ino` — Arduino source code

# \- `Report for Git.docx` — Full design report

# 

# \## Tools Used

# \- Arduino IDE

# \- Proteus (circuit simulation)

