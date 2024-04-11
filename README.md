
# Swapify

Swapify is a desktop application where users can exchange unwanted items with fellow users to get something they want. Swapify will revolutionize the online shopping market by allowing users to swap used items as a focus. The simple user interfaces of Swapify also make it an enjoyable and easy-to-use system that will promote user reuse. Swapify was made for budget-conscious consumers and environmentally-conscious individuals. 
 

## Authors

- Sudhan Dahake
- Saba Berenji
- Sierra Erb


## Features

- Fullscreen mode
- Log in
- Sign up 
- View listings
- Create Post
- Delete Post
- Request Swap 
- Help


## Installation Guide

This installation guide is mainly for the placement of the DLL file. Considering we have decided to keep the project open source we decided to include steps to make the DLL file accessible to our WPF Front-End application.

- If you do end up making changes to TCP_Client project, Build the project again.
- Go the directory where the solution is located and go inside Debug folder.
- You'll find a total of 4 files related to TCP_Client project (more if you decide to add additional DLL files in the application).
- Copy those 4 files and Head over to WPF_Front-End\bin\Debug\net6.0-windows.
- Place all the 4 files inside this net6.0-windows folder.
- Now WPF_Front-End can detect all the C++ interfaced functions.
- Run the project to make sure everything runs smoothly.
