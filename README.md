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
### Dithering  
#### Fixed Thresholding
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

#### Random Thresholding
To Build:  
```
cd random_thresholding
make random  
```  

To Run(for e.g. on ./random_thresholding/input_images/bridge.raw):  
```
cd random_thresholding
./random input_images/bridge.raw 501 332 1  
```

#### Dithering Matrix
To Build:  
```
cd dithering_matrix  
make dither  
```  

To Run(for e.g. on ./dithering_matrix/input_images/bridge.raw with a dithering matrix of dimension 2):  
```
cd dithering_matrix
./dither input_images/bridge.raw 501 332 1 2  
```  

### Error Diffusion
#### Floyd Steinberg Error Diffusion
To Build:  
```
cd fs_error_diffusion  
make fs  
```  

To Run(for e.g. on ./fs_error_diffusion/input_images/bridge.raw):  
```
cd fs_error_diffusion
./fs input_images/bridge.raw 501 332 1  
```

#### Jarvis, Judice and Ninke Error Diffusion
To Build:  
```
cd jjn_error_diffusion  
make jjn  
```  

To Run(for e.g. on ./jjn_error_diffusion/input_images/bridge.raw):  
```
cd jjn_error_diffusion
./jjn input_images/bridge.raw 501 332 1  
```