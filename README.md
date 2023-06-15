# USC-EE569-HW02
This repo is Homework-02 of EE-569(Digital Image Processing) completed at USC.  

Name: Tejas Acharya  
Class: EE-569  
Date: 15-06-2023  
Assignment: Homework 2  
    
OS: Linux Mint  

## Edge Detection  

### Sobel Edge Detector  
To Build:  
```
cd sobel_edge_detector
make sobel
```  

To Run(for e.g. on ./sobel_edge_detector/input_images/elephant.raw with 80 threshold):  
```
cd sobel_edge_detector
./sobel input_images/elephant.raw 321 481 3 80
```

## Digital Half-toning  

### Fixed Thresholding
To Build:  
```
cd fixed_thresholding
make fixed  
```  

To Run(for e.g. on ./fixed_thresholding/input_images/bridge.raw with 80 threshold):  
```
cd fixed_thresholding
./fixed input_images/bridge.raw 501 332 1 80  
```  

### Random Thresholding
To Build:  
```
cd random_thresholding
make random  
```  

To Run(for e.g. on ./random_thresholding/input_images/bridge.raw):  
```
cd random_thresholding
./random input_images/bridge.raw 501 332 1