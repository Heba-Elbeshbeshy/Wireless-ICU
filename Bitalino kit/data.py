import numpy as np
import matplotlib.pyplot as plt
from opensignalsreader import OpenSignalsReader
from scipy import signal

def filteremg(time, Signal, low_pass=3, sfreq=1000, high_band=20, low_band=450):
    """
    time: Time data
    emg: EMG data
    high: high-pass cut off frequency
    low: low-pass cut off frequency
    sfreq: sampling frequency
    """
    
    # normalise cut-off frequencies to sampling frequency
    high_band = high_band/(sfreq/2)
    low_band = low_band/(sfreq/2)
    
    # create bandpass filter for EMG
    b1, a1 = signal.butter(4, [high_band,low_band], btype='bandpass')
    
    # process EMG signal: filter EMG
    emg_filtered = signal.filtfilt(b1, a1, Signal)    
    
    # process EMG signal: rectify
    emg_rectified = abs(emg_filtered)
    
    # create lowpass filter and apply to rectified signal to get EMG envelope
    low_pass = low_pass/(sfreq/2)
    b2, a2 = signal.butter(4, low_pass, btype='lowpass')
    emg_envelope = signal.filtfilt(b2, a2, emg_rectified)
    #array_1D = emg_envelope.flatten()
    peaks, _ = signal.find_peaks(emg_envelope)
    print("Number of peaks=",len(peaks))
    # plot graphs
    fig = plt.figure()
    plt.subplot(1, 3, 1)
    plt.title('Unfiltered,' + '\n' + 'unrectified EMG')
    plt.plot(time, Signal)
    plt.xlabel('Time (sec)')
    plt.ylabel('EMG (a.u.)')
    
    plt.subplot(1, 3, 2)
    plt.title(('Filtered,' + '\n' + 'rectified EMG: ' + str(int(high_band*sfreq)) + '-' + str(int(low_band*sfreq)) + 'Hz'))
    plt.plot(time, emg_rectified)
    plt.xlabel('Time (sec)')

    plt.subplot(1, 3, 3)
    plt.title('Filtered,rectified,' + '\n' + 'EMG envelope: ' + str(int(low_pass*sfreq)) + ' Hz')
    plt.plot(time, emg_envelope)
    plt.xlabel('Time (sec)')

    plt.show()


emg=OpenSignalsReader('EMG2.txt')
Signal = emg.raw('RAW')
Signal = emg.signal('RAW')
time = np.array([i/1000 for i in range(0, len(Signal), 1)]) # sampling rate 1000 Hz
emg_correctmean=Signal-np.mean(Signal)
filteremg(time, emg_correctmean)
