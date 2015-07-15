# Emotion Detection

This will detect the emotions in your speech based on the MFCC, Delta, Delta-Delta & Prosodic Features (Fundamental Frequency) present in you voice. It makes use of GMM for classification. Try making models for basic emotions like happy, neutral, anger and sad. The emotion detection is text independent and speaker independent. But you need to train enough database in order to be speaker independent. 

You need following things in order to make use of this repository.

1. Octave (For plotting data.) (You can ignore this if you don't want to plot features. But you need to edit source file.)
2. em4gmm
	You need to download em4gmm from https://github.com/juandavm/em4gmm   
	```make ```  
	```make install```
3. Opemsmile
	You need to download opensmile from http://www.audeering.com/research/opensmile   
  
	Install it using  
	```sh buildStandalone.sh -p /usr/bin```  
4. You may also need to download libsound using  
	```sudo apt-get install libasound2-dev```  
  
