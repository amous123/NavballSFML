/******************************************************************************

Author: Alexander Moussa
Project: Using ksp and KOS to convert 3d vectors into compass headings and display
them in a navigation ball formal using sfml in 2d.

*******************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>       
#include <SFML/Graphics.hpp>
//#include "EllipseShape.cpp"
#include <windows.h>

#include <chrono>
#include <thread>
using namespace std;

float horizontalHeading = 0.0;
float verticalHeading = 0.0;
float rollAngle = 0.0;
static const float pi = 3.141592654f;

void updateNavballValues(){
    string line;
    int i = 0;
    string arr[3];
    ifstream navballValuesFile ("navballParameters.txt");
    if(navballValuesFile.is_open()){
        while(getline(navballValuesFile, line)){
            arr[i] = line;
            cout << arr[i] << '\n';
            i++;
            
        }
        
        rollAngle = -stoi(arr[2]);
        verticalHeading = stoi(arr[1]);
        horizontalHeading = stoi(arr[0]);
        navballValuesFile.close();
    } 
    else cout << "unable to open file" << '\n';
    
}

void displayNavballValues(){
  cout << "============" << endl;
  cout << "horizontal heading: " << horizontalHeading << endl;
  cout << "vertical heading: " << verticalHeading << endl;
  cout << "roll angle: " << rollAngle << endl;
  
}


float sphereSizeAdjustment(float original) {
    // 90 is the normal radius of a sphere.
    // 225 is the chosen radius of the drawn sphere.
    return (original / 90) * 225;
}

float degToRad(float deg) {
    return (deg / (180.0 / pi));
}


int main()
{
    using namespace this_thread; // sleep_for, sleep_until
    using namespace chrono; // nanoseconds, system_clock, seconds

    SetCurrentDirectoryA("E:\\SteamLibrary\\steamapps\\common\\Kerbal Space Program\\Ships\\Script\\");

    cout<<"Hello World!! \n";
    displayNavballValues();

    sf::Font font;
    if (!font.loadFromFile("D:\\Programming\\Visual Studio Files\\source\\repos\\NavballC++Project\\Libraries\\fonts\\arial.ttf"))
    {
        // error...
        cout << "error" << endl;
    }

    struct planeInfo{        // Declare a local structure 
        float horizontal, vertical, roll;
    };

    sf::RenderWindow window(sf::VideoMode(800, 600), "Title");

    sf::Event event;


    sf::Text text;
    text.setFont(font); // font is a sf::Font
    text.setString("000");
    text.setCharacterSize(24); // in pixels
    text.setPosition(205.f, 460.f);

    sf::ConvexShape convexPlaneIcon;
    sf::Vector2f iconPoints[10] = { 
        sf::Vector2f(225.f - 60.f, 225.f - 1.f),
        sf::Vector2f(225.f - 60.f, 225.f + 1.f),
        sf::Vector2f(225.f - 20.f, 225.f + 1.f),
        sf::Vector2f(225.f, 225.f + 21.f),
        sf::Vector2f(225.f + 20.f, 225.f + 1.f),
        sf::Vector2f(225.f + 60.f, 225.f + 1.f),
        sf::Vector2f(225.f + 60.f, 225.f - 1.f),
        sf::Vector2f(225.f + 20.f, 225.f - 1.f),
        sf::Vector2f(225.f, 225.f + 19.f),
        sf::Vector2f(225.f - 20.f, 225.f - 1.f)
         };

    // resize it to 5 points
    convexPlaneIcon.setPointCount(10);

    // define the points
    for (int pt = 0; pt < 10; pt++) {
        convexPlaneIcon.setPoint(pt, iconPoints[pt]);
   }
    convexPlaneIcon.setFillColor(sf::Color::Transparent);
    convexPlaneIcon.setOutlineThickness(3.f);
    convexPlaneIcon.setOutlineColor(sf::Color::Yellow);
           

    
    sf::ConvexShape convexSky;
    sf::ConvexShape convexLand;
    
    sf::Color blueSky(30, 180, 220);
    convexSky.setPointCount(200);
    convexSky.setFillColor(blueSky);
    convexSky.setOutlineThickness(0.f);

    sf::Color orangeLand(220, 140, 30);
    convexLand.setPointCount(200);
    convexLand.setFillColor(orangeLand);
    convexLand.setOutlineThickness(0.f);
    

    sf::RectangleShape NavballBox(sf::Vector2f(450.f, 450.f));
    sf::RectangleShape HeadingBox(sf::Vector2f(70.f, 40.f));
    sf::CircleShape NavballCircle(225.f);

    NavballBox.setFillColor(sf::Color::Transparent);
    NavballBox.setOutlineThickness(2.f);

    HeadingBox.setPosition(190.f, 452.f);
    HeadingBox.setFillColor(sf::Color::Transparent);
    HeadingBox.setOutlineThickness(2.f);
    
    NavballCircle.setFillColor(sf::Color::Transparent);
    NavballCircle.setOutlineThickness(1.f);


    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // loops infinitely
        for (int i = 0; i < 5; i++) {

            updateNavballValues();
            // testing values
            //rollAngle = 0;
            //verticalHeading = 0;
            //horizontalHeading = 180;
            displayNavballValues();
            

            text.setString( to_string((int)horizontalHeading));

            // y axis curves
            float a = 225;

            sf::VertexArray curvey1(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvey2(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvey3(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvey4(sf::PrimitiveType::LineStrip, 100);

            sf::VertexArray curveymini1(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curveymini2(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curveymini3(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curveymini4(sf::PrimitiveType::LineStrip, 100);

            float by1 = ((int)horizontalHeading) % 360;
            float by2 = (90 + (int)horizontalHeading) % 360;
            float by3 = (180 + (int)horizontalHeading) % 360;
            float by4 = (270 + (int)horizontalHeading) % 360;
            if (by1 >= 270) {
                by1 = by1 - 360;
            }
            if (by2 >= 270) {
                by2 = by2 - 360;
            }
            if (by3 >= 270) {
                by3 = by3 - 360;
            }
            if (by4 >= 270) {
                by4 = by4 - 360;
            }
            float by1adj = sphereSizeAdjustment(by1);
            float by2adj = sphereSizeAdjustment(by2);
            float by3adj = sphereSizeAdjustment(by3);
            float by4adj = sphereSizeAdjustment(by4);

            float bymini1 = (45 + (int)horizontalHeading) % 360;
            float bymini2 = (135 + (int)horizontalHeading) % 360;
            float bymini3 = (225 + (int)horizontalHeading) % 360;
            float bymini4 = (315 + (int)horizontalHeading) % 360;
            if (bymini1 >= 270) {
                bymini1 = bymini1 - 360;
            }
            if (bymini2 >= 270) {
                bymini2 = bymini2 - 360;
            }
            if (bymini3 >= 270) {
                bymini3 = bymini3 - 360;
            }
            if (bymini4 >= 270) {
                bymini4 = bymini4 - 360;
            }
            float bymini1adj = sphereSizeAdjustment(bymini1);
            float bymini2adj = sphereSizeAdjustment(bymini2);
            float bymini3adj = sphereSizeAdjustment(bymini3);
            float bymini4adj = sphereSizeAdjustment(bymini4);

            float q = rollAngle - 90; 
            float py = q / (180.0 / pi);

            curvey1.clear();
            curvey2.clear();
            curvey3.clear();
            curvey4.clear();
            curveymini1.clear();
            curveymini2.clear();
            curveymini3.clear();
            curveymini4.clear();



            //x axis curves
            sf::VertexArray curvexmain(sf::PrimitiveType::LineStrip, 100);

            sf::VertexArray curvexmedtop(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminitop1(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminitop2(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminitop3(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminitop4(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminitop5(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminitop6(sf::PrimitiveType::LineStrip, 100);

            sf::VertexArray curvexmedbottom(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminibottom1(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminibottom2(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminibottom3(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminibottom4(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminibottom5(sf::PrimitiveType::LineStrip, 100);
            sf::VertexArray curvexminibottom6(sf::PrimitiveType::LineStrip, 100);

            curvexmain.clear();

            curvexmedtop.clear();
            curvexminitop1.clear();
            curvexminitop2.clear();
            curvexminitop3.clear();
            curvexminitop4.clear();
            curvexminitop5.clear();
            curvexminitop6.clear();

            curvexmedbottom.clear();
            curvexminibottom1.clear();
            curvexminibottom2.clear();
            curvexminibottom3.clear();
            curvexminibottom4.clear();
            curvexminibottom5.clear();
            curvexminibottom6.clear();

            float bxmain = ((int)verticalHeading);

            float bxmedtop = ((int)verticalHeading + 45);
            float bxminitop1 = ((int)verticalHeading + 10);
            float bxminitop2 = ((int)verticalHeading + 20);
            float bxminitop3 = ((int)verticalHeading + 30);
            float bxminitop4 = ((int)verticalHeading + 60);
            float bxminitop5 = ((int)verticalHeading + 70);
            float bxminitop6 = ((int)verticalHeading + 80);

            float bxmedbottom = ((int)verticalHeading - 45);
            float bxminibottom1 = ((int)verticalHeading - 10);
            float bxminibottom2 = ((int)verticalHeading - 20);
            float bxminibottom3 = ((int)verticalHeading - 30);
            float bxminibottom4 = ((int)verticalHeading - 60);
            float bxminibottom5 = ((int)verticalHeading - 70);
            float bxminibottom6 = ((int)verticalHeading - 80);

            float bxmainadj = sphereSizeAdjustment(bxmain);

            float bxmedtopadj = sphereSizeAdjustment(bxmedtop);
            float bxminitopadj1 = sphereSizeAdjustment(bxminitop1);
            float bxminitopadj2 = sphereSizeAdjustment(bxminitop2);
            float bxminitopadj3 = sphereSizeAdjustment(bxminitop3);
            float bxminitopadj4 = sphereSizeAdjustment(bxminitop4);
            float bxminitopadj5 = sphereSizeAdjustment(bxminitop5);
            float bxminitopadj6 = sphereSizeAdjustment(bxminitop6);

            float bxmedbottomadj = sphereSizeAdjustment(bxmedbottom);
            float bxminibottomadj1 = sphereSizeAdjustment(bxminibottom1);
            float bxminibottomadj2 = sphereSizeAdjustment(bxminibottom2);
            float bxminibottomadj3 = sphereSizeAdjustment(bxminibottom3);
            float bxminibottomadj4 = sphereSizeAdjustment(bxminibottom4);
            float bxminibottomadj5 = sphereSizeAdjustment(bxminibottom5);
            float bxminibottomadj6 = sphereSizeAdjustment(bxminibottom6);

            float qx = rollAngle;
            float px = qx / (180.0 / pi);

            // parametric formula: ((a* cos(t))* cos(py) - (by1adj * sin(t)) * sin(py) + 225), ((by1adj* sin(t))* cos(py) + (a * cos(t)) * sin(py) + 225 ))
            // a: represents the scaling factor relative to a radius of 1.
            // p represents the angle rotation from -90 degrees to 90 degrees.
            // py starts at -90 degrees to represent the angle rotation vertically.
            // px starts at 0 degrees to represent the angle rotation horizontally.
            // 225 represents the horizontal and vertical transformation of the circle which is a constant 225.
            // t represents the parametric variable. To represent a semi ellipse, the value of t goes from 0 to pi. a full ellipse is 2pi.

            sf::Color grey(192, 192, 192);
            //convex.setPoint(0, sf::Vector2f(((a* cos(0))* cos(px) - (bxmainadj * sin(0)) * sin(px) + 225), ((bxmainadj* sin(0))* cos(px) + (a * cos(0)) * sin(px) + 225)));
            int convexSkyIndex = 0;
            int convexLandIndex = 0;

            for (float t = pi/100; t < pi; t+=pi/100)
            {

                curvey1.append(sf::Vertex(sf::Vector2f(((a* cos(t))* cos(py) - (by1adj * sin(t)) * sin(py) + 225), ((by1adj* sin(t))* cos(py) + (a * cos(t)) * sin(py) + 225 ))));
                curvey2.append(sf::Vertex(sf::Vector2f(((a* cos(t))* cos(py) - (by2adj * sin(t)) * sin(py) + 225), ((by2adj* sin(t))* cos(py) + (a * cos(t)) * sin(py) + 225))));
                curvey3.append(sf::Vertex(sf::Vector2f(((a* cos(t))* cos(py) - (by3adj * sin(t)) * sin(py) + 225), ((by3adj* sin(t))* cos(py) + (a * cos(t)) * sin(py) + 225))));
                curvey4.append(sf::Vertex(sf::Vector2f(((a* cos(t))* cos(py) - (by4adj * sin(t)) * sin(py) + 225), ((by4adj* sin(t))* cos(py) + (a * cos(t)) * sin(py) + 225))));

                curveymini1.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(py) - (bymini1adj * sin(t)) * sin(py) + 225), ((bymini1adj * sin(t)) * cos(py) + (a * cos(t)) * sin(py) + 225)), grey));
                curveymini2.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(py) - (bymini2adj * sin(t)) * sin(py) + 225), ((bymini2adj * sin(t)) * cos(py) + (a * cos(t)) * sin(py) + 225)), grey));
                curveymini3.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(py) - (bymini3adj * sin(t)) * sin(py) + 225), ((bymini3adj * sin(t)) * cos(py) + (a * cos(t)) * sin(py) + 225)), grey));
                curveymini4.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(py) - (bymini4adj * sin(t)) * sin(py) + 225), ((bymini4adj * sin(t)) * cos(py) + (a * cos(t)) * sin(py) + 225)), grey));

                
                curvexmain.append(sf::Vertex(sf::Vector2f(((a* cos(t))* cos(px) - (bxmainadj * sin(t)) * sin(px) + 225), ((bxmainadj * sin(t))* cos(px) + (a * cos(t)) * sin(px) + 225))));
                convexSky.setPoint(convexSkyIndex, sf::Vector2f(((a * cos(t)) * cos(px) - (bxmainadj * sin(t)) * sin(px) + 225), ((bxmainadj * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)));
                convexLand.setPoint(convexLandIndex, sf::Vector2f(((a * cos(t)) * cos(px) - (bxmainadj * sin(t)) * sin(px) + 225), ((bxmainadj * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)));
                convexSkyIndex++;
                convexLandIndex++;
             
            }

            double initialT = degToRad(0 + rollAngle);
            double targetT = degToRad(180 + rollAngle);

            cout << "initial: " << initialT << ", target: " << targetT << endl;
            int r = 225;           

            for (double t = targetT; t >= initialT; t -= pi / 100) {

                convexSky.setPoint(convexSkyIndex, sf::Vector2f(-r*cos(t) + 225, -r*sin(t) + 225));
                //cout << convexSkyIndex << " point is:  " << convexSky.getPoint(convexSkyIndex).x << " : " << convexSky.getPoint(convexSkyIndex).y << " ; t is:" << t << endl;
                convexSkyIndex++;

            }
            convexSky.setPoint(199, sf::Vector2f(-r * cos(initialT) + 225, -r * sin(initialT) + 225));

            for (float t = targetT; t > initialT; t -= pi / 100) {

                convexLand.setPoint(convexLandIndex, sf::Vector2f(r * cos(t) + 225, r * sin(t) + 225));
                convexLandIndex++;

            }         
            convexLand.setPoint(199, sf::Vector2f(r * cos(initialT) + 225, r * sin(initialT) + 225));




            for (float t = pi * 0.3; t <= pi * 0.7; t += pi / 50)
            {
                curvexmedtop.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxmedtopadj * sin(t)) * sin(px) + 225), ((bxmedtopadj * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225))));
                curvexmedbottom.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxmedbottomadj * sin(t)) * sin(px) + 225), ((bxmedbottomadj * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225))));
                
            }

            for (float t = pi * 0.44; t <= pi * 0.58; t += pi / 50)
            {
                
                curvexminitop2.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminitopadj2 * sin(t)) * sin(px) + 225), ((bxminitopadj2 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225))));
                curvexminitop5.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminitopadj5 * sin(t)) * sin(px) + 225), ((bxminitopadj5 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225))));

                curvexminibottom2.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminibottomadj2 * sin(t)) * sin(px) + 225), ((bxminibottomadj2 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225))));
                curvexminibottom5.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminibottomadj5 * sin(t)) * sin(px) + 225), ((bxminibottomadj5 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225))));

            }

            for (float t = pi * 0.46; t < pi * 0.56; t += pi / 50)
            {

                curvexminitop1.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminitopadj1 * sin(t)) * sin(px) + 225), ((bxminitopadj1 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)), grey));
                curvexminitop3.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminitopadj3 * sin(t)) * sin(px) + 225), ((bxminitopadj3 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)), grey));
                curvexminitop4.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminitopadj4 * sin(t)) * sin(px) + 225), ((bxminitopadj4 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)), grey));
                curvexminitop6.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminitopadj6 * sin(t)) * sin(px) + 225), ((bxminitopadj6 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)), grey));

                curvexminibottom1.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminibottomadj1 * sin(t)) * sin(px) + 225), ((bxminibottomadj1 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)), grey));
                curvexminibottom3.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminibottomadj3 * sin(t)) * sin(px) + 225), ((bxminibottomadj3 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)), grey));
                curvexminibottom4.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminibottomadj4 * sin(t)) * sin(px) + 225), ((bxminibottomadj4 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)), grey));
                curvexminibottom6.append(sf::Vertex(sf::Vector2f(((a * cos(t)) * cos(px) - (bxminibottomadj6 * sin(t)) * sin(px) + 225), ((bxminibottomadj6 * sin(t)) * cos(px) + (a * cos(t)) * sin(px) + 225)), grey));
               
            }

            // clear the window with black color
            window.clear(sf::Color::Black);

            if (verticalHeading > 0) {
                window.draw(convexLand);
                window.draw(convexSky);
            }
            else {
                window.draw(convexSky);
                window.draw(convexLand);
            }

            window.draw(NavballBox);
            window.draw(HeadingBox);
            window.draw(NavballCircle);

            window.draw(curvexmain);

            window.draw(text);


            //x axis bound checking
            if (-90 < bxmedtop && bxmedtop < 90) {
                window.draw(curvexmedtop);                
            }
            if (-90 < bxminitop1 && bxminitop1 < 90) {
                window.draw(curvexminitop1);
            }
            if (-90 < bxminitop2 && bxminitop2 < 90) {
                window.draw(curvexminitop2);
            }
            if (-90 < bxminitop3 && bxminitop3 < 90) {
                window.draw(curvexminitop3);
            }
            if (-90 < bxminitop4 && bxminitop4 < 90) {
                window.draw(curvexminitop4);
            }
            if (-90 < bxminitop5 && bxminitop5 < 90) {
                window.draw(curvexminitop5);
            }
            if (-90 < bxminitop6 && bxminitop6 < 90) {
                window.draw(curvexminitop6);
            }

            if (-90 < bxmedbottom && bxmedbottom < 90) {
                window.draw(curvexmedbottom);
            }
            if (-90 < bxminibottom1 && bxminibottom1 < 90) {
                window.draw(curvexminibottom1);
            }
            if (-90 < bxminibottom2 && bxminibottom2 < 90) {
                window.draw(curvexminibottom2);
            }
            if (-90 < bxminibottom3 && bxminibottom3 < 90) {
                window.draw(curvexminibottom3);
            }
            if (-90 < bxminibottom4 && bxminibottom4 < 90) {
                window.draw(curvexminibottom4);
            }
            if (-90 < bxminibottom5 && bxminibottom5 < 90) {
                window.draw(curvexminibottom5);
            }
            if (-90 < bxminibottom6 && bxminibottom6 < 90) {
                window.draw(curvexminibottom6);
            }

            // y axis bound checking
            if (-90 < by1 && by1 < 90) {
                window.draw(curvey1);
            }
            if (-90 < by2 && by2 < 90) {
                window.draw(curvey2);
            }
            if (-90 < by3 && by3 < 90) {
                window.draw(curvey3);
            }
            if (-90 < by4 && by4 < 90) {
                window.draw(curvey4);
            }

            if (-90 < bymini1 && bymini1 < 90) {
                window.draw(curveymini1);
            }
            if (-90 < bymini2 && bymini2 < 90) {
                window.draw(curveymini2);
            }
            if (-90 < bymini3 && bymini3 < 90) {
                window.draw(curveymini3);
            }
            if (-90 < bymini4 && bymini4 < 90) {
                window.draw(curveymini4);
            }

            window.draw(convexPlaneIcon);
            

            // end the current frame
            window.display();

            sleep_for(milliseconds(500));

        }    

    }

    return 0;
}
