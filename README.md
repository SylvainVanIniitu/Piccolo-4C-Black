# PiccoloBlack
MIDI Stochastic Sequencer

Project of a stochastic MIDI sequencer developped since a few years (2016) in solo
I decided in 2023 to make its code and the schematics public and open to modifications by others

Piccolo is a polyrythmic errorist and stochastic MIDI sequencer, slightly verging towards a proto-programmable interpreter, very apt to generative and process-based music.the current version is based on the Teensy 4.1 architecture and MIDI protocol.    The previous versions  were relying on Teensy 3.5 and earlier ATmega2560.   The step to Teensy3.5 had provided 10x more precision in time, and 10x more memory.   the best place to see some movies of the action of the current prototypes is on my fb page : https://www.facebook.com/Piccolo-MIDI-sequencer-230847087794581/ 

i also compiled a few videos here : https://www.youtube.com/watch?v=yQdNbUFVs8c

several records have been released as Sylvain van iniitu or 4LLP4 CH1N0, using Piccolo as its main core

the current software version running the sequencer, and fully written in C, features :

MIDI in
MIDI out
OLED screen ( 4 lines of 20 characters ), in place of LCD screen
tap tempo ( different styles )
4 time-independent layers
64 layer memories ( to store layers and load then when necessary )
Unlimited duration loops ( unless you consider a maximum of 4 years as a limitation.. ), that can be independent.   Precision of 1 msec.  full 16 Channel capacity ( 16 channel can be used on each 4 layers )
Unlimited notes by layer ( up till the time the memory is full… we haven’t got there yet when playing )
more than 100 matrix transformations of the sequences ( delay, variable delay, transposition, harmonization, modulation, tonulation, reverse, inverse, simplification, etc... ) - most of it is crazy stuff, you'll just need to dive in the manual to get a clue
modulation of the layers by sequences : useful for (ir)regular transpositions, by note or by layer, modification of velocity, modification of length of note, etc.
possibility to program regular events, launching a transformation, either in absolute time, or after a certain repetition of one (or several) layers
sending CC to any channel, either time-based ( ex. modifying the length of a reverb every 30 sec ), or by note ( modifying it every 5 notes )
possibility to save and load layers as .txt files on SD cards. you can simply edit those .txt files on any text editor on any platform
reading .MIDI files on the SD card ; possibility to glitch those at core level
USB cable alimentation - does not require a computer to work
non-destructive and non-permanent delays, under various forms : regular delay, quad delay ( adapted from Tambour ), fractal delay ( adapted from Dende )
step sequencer : material 4 steps, but virtual from 2 to 127 steps ; you can program it with any note from any channel

Big thanks to : Ikue Mori, Steve Reich, Two Lone Swordsmen, Christian Wolff, Seth Horvitz (Sutekh), Phillus, Burnt Friedman, Cylob and Actress for most inspiration on building this thing.

Please send me the music you make with this sequencer !

If you make a short run of them, please send me one !

If you want to support, please buy my music at bandcamp E42.A8 : https://e42a8.bandcamp.com 

and if you like it : buy some more at https://stilll-off.bandcamp.com/album/sylvain-van-iniitu-plays-django-reinhardt 

or http://tanukirecords.bandcamp.com/album/vulgar-display-of-debug-vol-3-thrift-thrift-horatio

and yes, invite me for a workshop somewhere, and let me play a concert in your city ! 

P x L x U x R

sylvain van iniitu 




