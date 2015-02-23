#include "ofApp.h"


float wscale = //1.0/2.5;

1;
int W = 1280;
int H = 720;
//--------------------------------------------------------------
void ofApp::setup() {
  //  ofSetWindowShape(1024/3,768/3);
    
    ofSetWindowTitle( "Video synth" );
    //ofSetWindowShape( 1280, 720 );
    ofSetWindowShape( W*wscale, H*wscale );

    ofBackground( ofColor::white );

    ofSetFrameRate( 60 );
   
//    ofSetFullscreen( true );
    
    gui.setup( "Parameters", "settings.xml" );
    gui.add( countX.setup( "countX", 50, 0, 200 ) );
    gui.add( stepX.setup( "stepX", 20, 0, 200 ) );
    gui.add( twistX.setup( "twistX", 5, -45, 45 ) );
    
    gui.add( countY.setup( "countY", 0, 0, 50) );
    gui.add( stepY.setup( "stepY", 20, 0, 200 ) );
    gui.add( twistY.setup( "twistY", 0, -30, 30 ) );
    gui.add( pinchY.setup( "pinchY", 0, 0, 1 ) );

    
    globalGroup.setup( "Global" );
    globalGroup.add( Scale.setup( "Scale", 1, 0.0, 1 ) );
    globalGroup.add( Rotate.setup( "Rotate", 0, -180, 180 ) );
    globalGroup.add( Background.setup( "Background", 255, 0, 255 ) );
    gui.add( &globalGroup );
  
    primGroup.setup( "Primitive" );
    primGroup.add( shiftY.setup( "shiftY", 0.0, -1000.0, 1000.0 ) );
    primGroup.add( rotate.setup( "rotate", 0.0, -180.0, 180.0 ) );
    primGroup.add( size.setup( "size",
                              ofVec2f(6,6),
                              ofVec2f(0,0),
                              ofVec2f(20,20) ) );
    
    //primGroup.add( lineWidth.setup( "lineWidth", 3.0, 0.0, 8.0 ) );
    primGroup.add( color.setup( "color",
                               ofColor::black,
                               ofColor(0,0,0,0),
                               ofColor::white ) );
    primGroup.add( filled.setup( "filled", false ) );
    primGroup.add( type.setup( "type", false ) );
    gui.add( &primGroup );
    
    //---------- chapter 4
    ofLoadImage( image, "collage.png" );
    video.loadMovie( "flowing.mp4" );
    video.play();
    //ofLoadImage( video, "flowing.png" );
    
    
    mixerGroup.setup( "Mixer" );
    mixerGroup.setHeaderBackgroundColor( ofColor::darkRed );
    mixerGroup.setBorderColor( ofColor::darkRed );
    
    mixerGroup.add( imageAlpha.setup( "image", 100, 0, 255 ) );
    mixerGroup.add( videoAlpha.setup( "video", 200, 0, 255 ) );
    mixerGroup.add( cameraAlpha.setup( "camera", 100, 0, 255 ) );
    
    shader.load( "kaleido" );

    mixerGroup.add( kenabled.setup( "kenabled", true ) );
    mixerGroup.add( ksectors.setup( "ksectors", 10, 1, 100 ) );
    mixerGroup.add( kangle.setup( "kangle", 0, -180, 180 ) );
    mixerGroup.add( kx.setup( "kx", 0.5, 0, 1 ) );
    mixerGroup.add( ky.setup( "ky", 0.5, 0, 1 ) );

    mixerGroup.add( show2d.setup("show2d", 255, 0, 255) );
    mixerGroup.add( show3d.setup("show3d", 255, 0, 255) );
    fbo3d.allocate( ofGetWidth(), ofGetHeight(), GL_RGBA );

    mixerGroup.add( rad.setup("rad", 250, 0, 500) );
    mixerGroup.add( deform.setup("deform", 0.3, 0, 1.5) );
    mixerGroup.add( deformFreq.setup("deformFreq", 3, 0, 10) );
    mixerGroup.add( extrude.setup("extrude", 1, 0, 5) );

    
    gui.minimizeAll();
    gui.add( &mixerGroup );
    
    
    fbo.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    
    //---------- chapter 5
    fbo2.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );

    //sphere.set(250, 20);
    sphere.set(250, 80);
    
    vertices0 = sphere.getMesh().getVertices();
    
    float w = fbo2.getWidth();
    float h = fbo2.getHeight();
    sphere.mapTexCoords( 0, h, w, 0 );
    sphere.rotate(180, 0, 1, 0);
    
    
    
  /*  _3DGroup.setup("_3D");
    _3DGroup.add( use3D.setup( "use3D", true ) );
    _3DGroup.add( texturing.setup( "texturing", true ) );
    _3DGroup.add( additive.setup( "additive", false ) );
    
    _3DGroup.add( deformAmp.setup( "deformAmp", 0, 0, 1 ) );
    _3DGroup.add( deformStep.setup( "deformStep", 0.1, 0.1, 3 ) );
    
    _3DGroup.add( morphSph.setup( "morphSph", 0, 0, 1 ) );
    gui.add( &_3DGroup );
*/
    
    //----------
    gui.loadFromFile( "settings.xml" );
    
//    showGui = true;
    showGui = false;    //chapter 5

    
}


//--------------------------------------------------------------
void ofApp::exit() {
    gui.saveToFile( "settings.xml" );
}

//--------------------------------------------------------------
float time0 = 0;
void ofApp::update(){
    
    video.update();

    if ( camera.isInitialized() ) camera.update();

    vector<ofPoint> &vertices = sphere.getMesh().getVertices();
    
    for (int i=0; i<vertices.size(); i++) {
        ofPoint v = vertices0[i]; //.getNormalized();
        v.normalize();
        float sx = sin( v.x * deformFreq );
        float sy = sin( v.y * deformFreq );
        float sz = sin( v.z * deformFreq );
        
        v.x += sy * sz * deform;
        v.y += sx * sz * deform;
        v.z += sx * sy * deform;

        v *= rad;
    
        vertices[i] = v;
    }
    

    ofPixels pixels;
    fbo2.readToPixels(pixels);
    
    //vector<ofVec2f> &tcoords = sphere.getMesh().getTexCoords();
    for (int i=0; i<vertices.size(); i++) {
        ofVec2f t = sphere.getMesh().getTexCoords()[i];  //tcoords[i];
        t.x = ofClamp( t.x, 0, pixels.getWidth()-1 );
        t.y = ofClamp( t.y, 0, pixels.getHeight()-1 );
        float br = pixels.getColor(t.x, t.y).getBrightness();
        vertices[i] *= 1 + br / 255.0 * extrude;
    }
    
    //Normals
    
 //   vector<ofVec3f> normals = mesh.getFaceNormals( false );
 //   mesh.clearNormals();
 //   mesh.addNormals( normals );
    
}


//--------------------------------------------------------------
void ofApp::stripePattern() {
    for (int i=-countX; i<=countX; i++) {
        ofPushMatrix();
        ofTranslate( i * stepX, 0 );
        ofRotate( i * twistX );
//        ofScale( 6, 6 );
//        ofTriangle( 0, 0, -50, 100, 50, 100 );

//        ofSetLineWidth( lineWidth );
        
        ofTranslate( 0, shiftY );
        ofRotate( rotate );
        ofScale( size->x, size->y );
        ofSetColor( color );
        if ( filled ) ofFill(); else ofNoFill();
        if ( type ) ofRect( -50, -50, 100, 100 );
        else ofTriangle( 0, 0, -50, 100, 50, 100 );
        
        
        ofPopMatrix();
    }

/*    for (int i=-50; i<=50; i++) {
        ofPushMatrix();
        ofTranslate( i * 20, 0 );
        ofRotate( i * 5 );
        ofScale( 6, 6 );
        ofTriangle( 0, 0, -50, 100, 50, 100 );
        ofPopMatrix();
    }*/
}

//--------------------------------------------------------------
void ofApp::matrixPattern() {
    for (int y=-countY; y<=countY; y++) {
        ofPushMatrix();
        //---------------------
        float scl = ofMap( y, -countY, countY, 1-pinchY, 1 );
        ofScale( scl, scl );
        ofTranslate( 0, y * stepY );
        ofRotate( y * twistY );
        //---------------------
        stripePattern();
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::draw1(){
    ofBackground( Background );
    
    //---- chapter 4
    ofDisableSmoothing();
    //ofEnableBlendMode( OF_BLENDMODE_ADD );
    
    ofSetColor( 255, imageAlpha );
    image.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    
    ofSetColor( 255, videoAlpha );
    video.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    
    if ( camera.isInitialized() ) {
        ofSetColor( 255, cameraAlpha );
        camera.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    }
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    //    ofBackground( 255 );
    
    //----
    
    
    ofPushMatrix();
    ofScale( wscale, wscale );
    
    ofTranslate( W / 2, H / 2 );
    
    float Scl = pow( Scale, 4.0f );
    ofScale( Scl, Scl );
    ofRotate( Rotate );
    
    ofSetColor( ofColor::black );
    ofSetLineWidth( 1.0 );
    
    matrixPattern();
    
    ofPopMatrix();
    

}


//--------------------------------------------------------------
void ofApp::draw3d() {
    
    
    fbo2.getTextureReference().bind();
    
    //autocamera
    //float time = ofGetElapsedTimef();
    //float longitude = 10*time;
    //float latitude = 10*sin(time*0.8);
    //float radius = 600 + 50*sin(time*0.4);
    //cam.orbit( longitude, latitude, radius, ofPoint(0,0,0) );
    
    
  //  ofEnableLighting();
    light.setPosition(ofGetWidth()/2, ofGetHeight()/2, 600);
    light.enable();
    material.begin();
    ofEnableDepthTest();
    
    cam.begin();
    ofSetColor( ofColor::white );
    
    //sphere.drawWireframe();
    sphere.draw();

    cam.end();

    ofDisableDepthTest();
    material.end();
    light.disable();
    ofDisableLighting();
    
    fbo2.getTextureReference().unbind();

}

//--------------------------------------------------------------
void ofApp::draw(){
    fbo.begin();
    draw1();
    fbo.end();
    
    fbo2.begin();   //--- chapter 5
    if ( kenabled ) {
        shader.begin();
        shader.setUniform1i( "ksectors", ksectors );
        shader.setUniform1f( "kangleRad", ofDegToRad( kangle ) );
        shader.setUniform2f( "kcenter", kx*ofGetWidth(), ky*ofGetHeight() );
        shader.setUniform2f( "screenCenter", 0.5*ofGetWidth(), 0.5*ofGetHeight() );
    }
    
    ofSetColor( ofColor::white );
    fbo.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    if ( kenabled ) shader.end();
    fbo2.end();   //---- chapter 5
    
    //--- chapter 5

    fbo3d.begin();
    ofBackground( 0, 0 );
    draw3d();
    fbo3d.end();
    
    ofBackground( 0 );
    ofSetColor( 255, show2d );
    fbo2.draw( 0, 0 );
    ofSetColor( 255, show3d );
    fbo3d.draw( 0, 0 );
    
    
  //  draw3d();
    
    /*
    ofBackground( 0 );
    
    if (use3D ) {
     
        
    }
    else {
        ofSetColor( ofColor::white );
        fbo2.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    }
     */

    
    
    //------
    if ( showGui ) gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    /*if ( key == '1' ) {
        globalGroup.toggleMinimized();
        primGroup.toggleMinimized();
    }
    if ( key == '2' ) {
        mediaGroup.toggleMinimized();
    }*/
    

    if ( key == 'z' ) { showGui = !showGui;
        if ( showGui ) cam.disableMouseInput();
        else cam.enableMouseInput();
    }

    
    
    if ( key == OF_KEY_RETURN ) ofSaveScreen( "screenshot.png" );
    
    if ( key == 's' ) {
        ofFileDialogResult res;
        res = ofSystemSaveDialog( "preset.xml", "Saving Preset");
        if ( res.bSuccess ) gui.saveToFile( res.filePath );
    }
    if ( key == 'l' ) {
        ofFileDialogResult res;
        res = ofSystemLoadDialog( "Loading Preset" );
		if ( res.bSuccess ) gui.loadFromFile( res.filePath );
    }
    
    if ( key == 'c' ) {
        camera.setDeviceID( 0 );
        camera.setDesiredFrameRate( 30 );
        camera.initGrabber( 1280, 720 );
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
