# Virtual Museum Project

## Overview
The Virtual Museum Project is an interactive 3D application that simulates a museum environment where users can explore and learn about historical artifacts from the Adana Museum. This application combines 3D modeling, real-time rendering, and informational content to provide an immersive educational experience.

## Features
- Interactive 3D environment with realistic lighting
- Robot-guided exploration with manual and automatic touring options
- Five detailed historical artifacts with complete information in both Turkish and English
- Scanning functionality that provides detailed information about each artifact
- Ambient lighting controls to adjust the viewing experience
- Intuitive user interface with popup information windows

## Technical Details

### Built With
- C++ (C++14)
- OpenGL 3.3
- GLFW (OpenGL Framework)
- GLM (OpenGL Mathematics)
- Dear ImGui (Immediate Mode GUI)
- GLAD (OpenGL Loading Library)
- STB Image (Image Loading)

### Key Components
1. **Main Application**: Handles window management, scene rendering, and user input
2. **Robot Control System**: Implements navigation, animation, and scanning functionality
3. **3D Model Management**: Loads and renders artifact models and pedestals
4. **Camera System**: Provides user viewport controls and movement
5. **User Interface**: Displays information and control panels using ImGui

## Critical Functions

### Core Application Functions
- `main()`: The entry point and main program loop 
- `framebuffer_size_callback()`: Handles window resizing events
- `processInput()`: Processes keyboard input for robot movement

### Robot Control Functions
- `Robot::update()`: Updates the robot's position, rotation, and animation
- `Robot::moveTo()`: Moves the robot to a target position
- `Robot::pointArmTo()`: Directs the robot's arm toward a specific position
- `Robot::startModelTour()`: Begins an automated tour of all artifacts

### Rendering Functions 
- Model, camera, and shader management classes handle the rendering pipeline
- `glm` library functions manage transformations and camera perspective
- `ImGui` functions create and manage the user interface elements

## Installation and Setup

### System Requirements
- GPU with OpenGL 3.3+ support
- Windows operating system (Windows 10/11 recommended)
- Visual Studio 2019 or 2022 with C++ development workloads installed

### Download and Setup Instructions
1. **Download the Project**
   - Download the project from this GitHub repository:
     [https://github.com/S3zgin4/Virtual-Museum.git]
   - Click the green "Code" button and select "Download ZIP"
   - Extract the ZIP file to a location on your computer

2. **Open in Visual Studio**
   - Open Visual Studio 2019/2022
   - Select "File" → "Open" → "Project/Solution"
   - Navigate to the extracted project folder
   - Select the "VirtualMuseumProject.sln" file and open it

3. **Configure Project Settings**
   - Make sure the configuration is set to "Release" in the top toolbar
   - Set the platform to "x64" 

4. **Build and Run**
   - Press F5 or click the "Local Windows Debugger" button to build and run the application
   - Alternatively, select "Build" → "Build Solution" (Ctrl+Shift+B) and then run the executable

## User Guide

### Manual Navigation
1. Use the **arrow keys** to move the robot around the museum:
   - Up arrow: Move forward
   - Down arrow: Move backward
   - Left arrow: Move left
   - Right arrow: Move right
2. Navigate the robot close to an artifact and remain stationary for a few seconds
3. The robot will automatically begin scanning the artifact
4. When scanning completes, detailed information about the artifact will appear
5. Click "Next" to close the information panel and continue exploring

### Automated Tour
1. Click the "Start Tour" button in the Virtual Museum panel
2. The robot will automatically move to each artifact in sequence
3. At each artifact, the robot will:
   - Position itself for optimal viewing
   - Scan the artifact
   - Display detailed information
4. Click "Next" after reading the information to proceed to the next artifact
5. Click "Stop Tour" at any time to end the automated tour

### Lighting Controls
1. Use the "Lighting Controls" panel in the upper right corner
2. Adjust the "Ambient Light" slider to change the overall brightness of the scene

### Exiting the Application
- Press the **ESC** key at any time to exit the application

## Artifacts in the Museum
1. **Erkek Heykeli / Statue of a Man**
   - Roman Period, 2nd century AD
   - Bronze statue depicting a man in a toga

2. **Lahit - Aslanli / Sarcophagus with Lion Reliefs**
   - Roman Period, 2nd century AD
   - Marble sarcophagus with lion reliefs

3. **Arabali Tanri Tarhunda / Chariot-Borne God Tarhunda**
   - Late Hittite Period, 9th century BC
   - Basalt stone statue of the storm god

4. **Lahit - Figurlu / Sarcophagus with Mythological Figures**
   - Roman Period, 2nd-3rd century AD
   - Marble sarcophagus with mythological scenes

5. **Kadin Rolyefli Mezar Steli / Funerary Stele with Female Relief**
   - Roman Period, 2nd-3rd century AD
   - Limestone funerary stele with female portrait
  
### Images and GIFs of UI and Models
**General View** <br/><br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/GeneralViewGIF.gif)

**Movement of Robot** <br/><br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/MoveRobot.gif)

**Lighting Settings** <br/><br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/LightingSettingsGIF.gif)

1. **Erkek Heykeli / Statue of a Man** <br/><br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/Model1GIF.gif)

2. **Lahit - Aslanli / Sarcophagus with Lion Reliefs** <br/><br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/Model2GIF.gif)

3. **Arabali Tanri Tarhunda / Chariot-Borne God Tarhunda** <br/><br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/Model3GIF.gif)

4. **Lahit - Figurlu / Sarcophagus with Mythological Figures** <br/><br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/Model4GIF.gif)

5. **Kadin Rolyefli Mezar Steli / Funerary Stele with Female Relief** <br/><br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/Model5GIF.gif)


**Image Gallery** <br/><br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/InfoPopUpImage.jpg) <br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/ScanningImage.jpg) <br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/RobotImage.png) <br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/TourMenuImage.png) <br/>
![](https://github.com/S3zgin4/Virtual-Museum/blob/main/Images-and-GIFs/LightSettingImage.png) 
<br/><br/>
**You can also check the Overview-Video-Museum file in the repository to watch the final overview of our Virtual Museum Project.** <br/>



## Troubleshooting
- **Model Loading Issues**: Ensure all model files are in the Models folder
- **Missing Textures**: Check that texture files (wall_orijinal.png) are in the correct directory
- **Build Errors**: Make sure Visual Studio has the C++ development workload installed
- **OpenGL Errors**: Update your graphics drivers to ensure OpenGL 3.3+ support
- **Performance Issues**: Adjust ambient lighting to improve visibility and performance

## Contributors
- This Virtual Museum Project was created as an educational application showcasing historical artifacts from the Adana Museum.


## License
- This project is an educational application created for demonstration purposes.

<br/> <br/> <br/>



