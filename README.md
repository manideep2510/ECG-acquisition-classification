# ECG-acquisition-classification
Single Lead ECG signal Acquisition and Arrhythmia Classification using Deep Learning

This project contain two parts,

1. ECG signal Acquasition

2. Arrhythmia classification of the acquired ECG signal.

## Files

- [`ecg`](ecg) : This folder contains all the files for training and testing of the deep learning algorithm and this code will be used to classifiy the acquired signals.

- [`arduino`](arduino) : Aurdino code for the 4th order Notch filter, displaying ECG signal in computer, calculating instantanious BPM, etc.

- [`classification.ipynb`](classification.ipynb) : Code to automaticaly create a file of the acquired ECG signal by reading from the serial moniter, convert that into required format and classify.

# About ECG Singnal Acquasition: Filters, Amplifiers, their circuits and their values

Electrocardiogram (ECG) is a recording - a graph of voltage versus time - of the electrical activity of the heart using electrodes placed on the skin. These electrodes detect the small electrical changes that are a consequence of cardiac muscle depolarization followed by repolarization during each cardiac cycle (heartbeat).

The prototype developed here focuses on the acquisition of the heart’s electrical activity with a simple electrical circuit. This electrocardiograph joins Arduino features with analog circuit and acquires, amplifies and filters a very weak signal obtained from the body.

The acquired ECG signal is used to monitor and display the heart rate in real time. The following cardiac cycle shows the generation of different waves in an ecg signal.

- The P-wave represents the wave of depolarization that spreads from the SA node throughout the atria.

- The QRS complex reflects the rapid depolarization of right and left ventricles. Since the ventricles are the largest parts of the heart, in terms of mass, the QRS complex usually has much larger amplitude than the P-wave.

- The T-wave represents the ventricular repolarization of the ventricles.

**Image**

### Block Diagram

**Image**

### Discussions on Design with Illustrations; Circuit Connections

Single Channel acquisition is done with electrodes on both the arms which are used to drive the circuit above. Reference is given to the right leg. The signal has an amplitude between 1 and 5mV which are difficult to visualize directly. For this reason, the system was designed to have a gain of around 1000.

The bandwidth of a normal ECG is not very wide and the fundamental frequency is approximately 1 Hz. The harmonic content decreases until is practicably negligible beyond 60Hz however, the system higher cutoff frequency was fixed to be 150Hz as part of the QRS complex is lost if filtered at lower cutoff frequencies.

**image**

ECG signal gets interfered by many artifacts which need to be filtered out. Main sources of noise in the ECG signal are Baseline wander (low-frequency noise), Power line  interference  
( 50Hz noise from a power line), Muscle noise (EMG interference) an electrical interference from other equipment. Various filters are used to remove these disturbances.

To remove the common mode noise INA128U (Instrumentation Amplifier) was used having a high CMRR of 120db. The gain of the amplifier was set to 29 (so that output will be between +9 and -9)

**Images**

To remove the baseline noise a high pass filter of cutoff frequency 0.05 Hz is used. A second order Butterworth filter was designed using opamp OPA27, a low noise amplifier to reduce noise amplification. The function of this filter is to eliminate the direct voltage in the signal and the resulting signal would be between + and - 5mv. 

**Images**

An amplification stage with a gain of 41. This gives a total gain of 1255.

**Images**

A low pass filter with 3db bandwidth at 150 Hz. This ensures unwanted frequencies in the ECG spectrum to be filtered off.

**Images**

Power line interference(50 Hz) is a major design artifact. To remove this an Arduino(nano) was programmed to function as a second order IIR notch filter of 2Hz bandwidth. Cutoff frequencies are 48Hz and 52Hz. Coefficients are derived from Matlab.

y = 0.6022 * x - 0.6022 * x1 + 0.6022 * x2 + 0.6022 * y1 - 0.2043 * y2;

X is the output from the low pass filter.
Y is the output from the notch filter.


**Heart Rate Monitor:**

The time interval between two high amplitude R waves (R-R Interval) is used to calculate the instantaneous heart rate. Thresholding was done in Arduino to detect the R peaks and thereby calculate heart rate.

A 16x2 LCD display is connected to Arduino to display beats per minute(BPM).

### Results

Single lead ECG detection using three electrodes was performed and the resulting ECG signal after passing through the cascade of the above filters was visualized using Arduino Serial plotter. Signal was sampled at a frequency of 500Hz. The entire circuit is assembled on a breadboard and powered through two 9v batteries making the prototype portable. Any abnormal heartbeat is indicated to the subject. The placement of electrodes also plays a role in the generation of different waves. We placed the positive electrode on the left hip, a negative electrode on the right shoulder and a electrode from the left hand wrist is given as a ground reference. This placement of electrodes is along the axis of heart alignment. Our observed ECG is 

**Image**

Here we take a look at each individual heartbeat, plotting all the 70 heartbeats in the sampled data (9000 data points) synchronized by their R peak, looks like below.

**Image**

Here is the variation of Heart Rate throughout the sampled data,

**Image**
