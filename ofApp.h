#pragma once

#include "ofMain.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void stripePattern();
    void matrixPattern();
    
    ofxPanel gui;
    
    ofxIntSlider countX;
    ofxFloatSlider stepX;
    ofxFloatSlider twistX;
    
    ofxIntSlider countY;
    ofxFloatSlider stepY, twistY, pinchY;
    
    
    ofxGuiGroup globalGroup;
    ofxFloatSlider Scale;
    ofxFloatSlider Rotate;
    ofxFloatSlider Background;

    ofxGuiGroup primGroup;
    ofxFloatSlider shiftY, rotate; //, lineWidth;
    ofxVec2Slider size;
    ofxColorSlider color;
    ofxToggle filled, type;
    
    bool showGui;
    
    void exit();
    
    //---- chapter 4 ----
    ofTexture image;
    ofVideoPlayer video;
    //ofTexture video;
    ofVideoGrabber camera;
    
    ofxGuiGroup mixerGroup;
    ofxFloatSlider imageAlpha, videoAlpha, cameraAlpha;
    
    ofFbo fbo;
    void draw1();

    ofShader shader;
    ofxToggle kenabled;
    ofxIntSlider ksectors;
    ofxFloatSlider kangle, kx, ky;
    

    //----- chapter 5 ----
    //ofPlanePrimitive plane;
    ofSpherePrimitive sphere;
    void draw3d();
    ofEasyCam cam;
    
    ofLight light;
    ofMaterial material;
    
    
    ofFbo fbo2;
    
    //ofPlanePrimitive planeTemp;
    
    //ofxGuiGroup _3DGroup;
    ofFbo fbo3d;
    
    ofxFloatSlider show2d, show3d;
    ofxFloatSlider rad, deform, deformFreq, extrude;
    
    vector<ofPoint> vertices0;
    
    //ofSpherePrimitive sph1;
    //ofIcoSpherePrimitive sph2;
    
    
    

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
		
};
