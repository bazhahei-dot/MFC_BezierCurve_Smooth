GIS  
C++程序设计作业  
MFC实现的Bezier拟合程序   
具备基本的绘制、放大、移动功能 ，核心功能实现了通过贝塞尔曲线对折线进行拟合  
基本流程：首先获取目标要素（折线），对要素进行预处理，得到点集，然后根据最小二乘法求出控制点，根据偏差与阈值的大小探索拟合三阶贝塞尔曲线，经过不断的探索过程，最后折线由若干拟合的三阶贝塞尔曲线代替，并传输显示在客户端上。  
![Aaron Swartz](https://user-images.githubusercontent.com/63851151/126943183-c4ac2627-cb15-46c4-b6a9-e493e721bbbd.png)
