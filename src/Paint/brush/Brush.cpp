#include "brush/Brush.h"
#include "brush/BrushShaders.h"


Brush::Brush()
{
    setupBrush();

    colorFbo.allocate(MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA);

    //setColorTexture(_colorTexture);


    params.setName("brush parameters");

    //ofAddListener(ofEvents().mousePressed , this, &Brush::onMouseDown);
    //ofAddListener(ofEvents().mouseScrolled, this, &Brush::onMouseScrolled);
    pColor.addListener( this, &Brush::onColorChanged);

    params.add(pBristleLength.set("Length"       , 3.0  , 0.0, 10.0 )); 
    params.add(pBristleJitter.set("Jitter"       , 0.85 , 0.0, 1.0  ));
    params.add(pIterations.set   ("Iterations"   , 12   , 1  , 20   ));
    params.add(pGravity.set      ("Gravity"      , 30.0, 0.0, 100.0));
    params.add(pDamping.set      ("Damping"      , 0.75, 0.0, 1.0  ));
    params.add(pStiffnessVariation.set("Stiffness Variation", 0.4, 0.0, 1.0));
    params.add(pHeight.set       ("Height"       , 2.0 , 0.0, 5.0  ));
    params.add(pScale.set        ("Scale"        , 100.0 , 50.0, 400.0  ));
    params.add(pColorMode.set    ("Color Mode", (int)ColorMode::SAMPLE, (int)ColorMode::SIMPLE_COLOR, (int)ColorMode::SAMPLE_CONTINIOUS));
    params.add(pVScale.set       ("V Scale"   , glm::vec2(0, 1), glm::vec2(0), glm::vec2(1)));
	params.add(pColor.set        ("color"     , ofFloatColor(1.0,1.0,.0,.5)));
			

}

Brush::~Brush()
{
    //ofRemoveListener(ofEvents().mousePressed , this, &Brush::onMouseDown   );
    //ofRemoveListener(ofEvents().mouseScrolled, this, &Brush::onMouseScrolled);
}

void Brush::setupBrush()
{
    ofDisableArbTex();
    // Setup  Shaders
    setupShader(projectProgram           , Shaders::fullscreenVert, Shaders::projectFrag            );
    setupShader(distanceConstraintProgram, Shaders::fullscreenVert, Shaders::distanceconstraintFrag );
    setupShader(planeConstraintProgram   , Shaders::fullscreenVert, Shaders::planeconstraintFrag    );
    setupShader(bendingConstraintProgram , Shaders::fullscreenVert, Shaders::bendingconstraintFrag  );
    setupShader(setBristlesProgram       , Shaders::fullscreenVert, Shaders::setbristlesFrag        );
    setupShader(updateVelocityProgram    , Shaders::fullscreenVert, Shaders::updatevelocityFrag     );
    setupShader(brushProgram             , Shaders::brushVert     , Shaders::brushFrag);

    // Setup Fbos
    positionsFbo              = make_shared<ofFbo>();
    previousPositionsFbo      = make_shared<ofFbo>();
    velocitiesFbo             = make_shared<ofFbo>();
    previousVelocitiesFbo     = make_shared<ofFbo>();
    projectedPositionsFbo     = make_shared<ofFbo>();
    projectedPositionsFboTemp = make_shared<ofFbo>();
    debugFbo                  = make_shared<ofFbo>();


    setupFbo(positionsFbo             , MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA32F );
    setupFbo(previousPositionsFbo     , MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA32F );
    setupFbo(velocitiesFbo            , MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA32F );
    setupFbo(previousVelocitiesFbo    , MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA32F );
    setupFbo(projectedPositionsFbo    , MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA32F );
    setupFbo(projectedPositionsFboTemp, MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA32F );
    setupFbo(debugFbo                 , MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA32F );

    // Setup brush buffer

    vector<glm::vec2> brushTextureCoordinates;
    vector<glm::vec3> brushVerts;
    for (int bristle = 0; bristle < MAX_BRISTLE_COUNT; ++bristle) {
        for (int vertex = 0; vertex < VERTICES_PER_BRISTLE; ++vertex) {

            float textureX = (bristle + 0.5) / MAX_BRISTLE_COUNT;
            float textureY = (vertex  + 0.5) / VERTICES_PER_BRISTLE;

            brushTextureCoordinates.push_back(glm::vec2(textureX, textureY));
            brushVerts.push_back(glm::vec3(bristle * 20, vertex * 20, 0.0));
        }
    }
    
    vector<unsigned int> brushIndices;
    indexCount = 0;

    for (int bristle = 0; bristle < MAX_BRISTLE_COUNT; ++bristle) {
        for (int vertex = 0; vertex < VERTICES_PER_BRISTLE - 1; ++vertex) {
            unsigned int left  = bristle * VERTICES_PER_BRISTLE + vertex;
            unsigned int right = bristle * VERTICES_PER_BRISTLE + vertex + 1;

            brushIndices.push_back(left);
            brushIndices.push_back(right);

            indexCount += 2;
        }
    }
    brushBuffer.setVertexData(&brushVerts[0]  , brushVerts.size()  , GL_STATIC_DRAW);
    brushBuffer.setIndexData( &brushIndices[0], brushIndices.size(), GL_STATIC_DRAW);
    brushBuffer.setTexCoordData(&brushTextureCoordinates[0], brushTextureCoordinates.size(), GL_STATIC_DRAW);

    //Setup random texture
    vector<float> randoms;
    for (int i = 0; i < MAX_BRISTLE_COUNT * VERTICES_PER_BRISTLE * 4; ++i) {
        randoms.push_back(ofRandom(0.02, 0.98));
    }
    randomsTexture.allocate( MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA32F);
    randomsTexture.loadData(&randoms[0], MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE, GL_RGBA);


    vector<glm::vec2> quadVerticies{
        {-1.0, -1.0 },
        {-1.0,  1.0 },
        { 1.0, -1.0 },
        { 1.0,  1.0 },
    };    
    quadVertexBuffer.setVertexData(&quadVerticies[0], quadVerticies.size(), GL_STATIC_DRAW);


    vector<glm::vec2> quadRowVerticies{
        {-1.0, -1.0},
        {-1.0, -1.0 + (2.0 / VERTICES_PER_BRISTLE) },
        { 1.0, -1.0 + (2.0 / VERTICES_PER_BRISTLE) },
        { 1.0, -1.0 },
    };
    
  
    quadRowVertexBuffer.setVertexData(&quadRowVerticies[0], quadRowVerticies.size(), GL_STATIC_DRAW);
    
    ofEnableArbTex();
}

void Brush::init(glm::vec2 pos)
{
    ofDisableArbTex();
    this->position = glm::vec3(pos.x, pos.y, pHeight * pScale);

    setBristles();
    ofEnableArbTex();
}

void Brush::drawBrush()
{
    ofDisableArbTex();

    brushProgram.begin();

    brushProgram.setUniform4f("u_color", glm::vec4(0.6, 0.6, 0.6, 1.0));
    brushProgram.setUniformTexture("u_positionsTexture", positionsFbo->getTexture(), 0);

    brushBuffer.drawElements( GL_LINES, indexCount );

    brushProgram.end();
    
    ofEnableArbTex();

}

void Brush::update(glm::vec2 pos)
{
    if (pColorMode == (int)ColorMode::SAMPLE_CONTINIOUS) {
        sampleShader.update(colorFbo, positionsFbo->getTexture(), colorTexture, pVScale);
    }


    ofDisableArbTex();

    glm::vec3 newPosition = glm::vec3(pos.x, pos.y, pHeight * pScale);

    speed = glm::length(position - newPosition);

    position = newPosition;

    project();

    setBristlesProjected();
    //
    for (int i = 0; i < pIterations; ++i) {
        setBristlesProjected(); 

        for (int pass = 0; pass < 2; ++pass) {
            distanceContrain(pass);
            swapTextures(projectedPositionsFbo, projectedPositionsFboTemp);
        }

        for (int pass = 0; pass < 3; ++pass) {
            blendingContrain(pass);
            swapTextures(projectedPositionsFbo, projectedPositionsFboTemp);
        }

        planeContrain();
        swapTextures(projectedPositionsFbo, projectedPositionsFboTemp);
    }

    updateVelocities();
    
    swapTextures(velocitiesFbo       , previousVelocitiesFbo);
    swapTextures(previousPositionsFbo, positionsFbo);
    swapTextures(positionsFbo        , projectedPositionsFbo);

    ofEnableArbTex();
}

void Brush::setupFbo(shared_ptr<ofFbo> fbo, int w, int h, int internalformat)
{
    fbo->allocate( w, h, internalformat );
    fbo->begin();
    ofClear(255.0, 255.0, 255.0, 255.0);
    fbo->end();

}

void Brush::setupShader(ofShader& shader, const string & vertSource, const string & fragSource)
{
    shader.setupShaderFromSource(GL_VERTEX_SHADER, vertSource);
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragSource);
    shader.bindDefaults();
    shader.linkProgram();
}

void Brush::swapTextures(shared_ptr<ofFbo> & fbo0, shared_ptr<ofFbo> & fbo1)
{
    shared_ptr<ofFbo> temp = fbo0;
    fbo0 = fbo1;
    fbo1 = temp;
}

void Brush::setBristles() {
    positionsFbo->begin();
    setBristlesProgram.begin();

    setBristlesProgram.setUniform3f( "u_brushPosition"     , position             );
    setBristlesProgram.setUniform1f( "u_brushScale"        , pScale                );
    setBristlesProgram.setUniform1f( "u_bristleCount"      , MAX_BRISTLE_COUNT    );
    setBristlesProgram.setUniform1f( "u_bristleLength"     , pBristleLength       );
    setBristlesProgram.setUniform1f( "u_verticesPerBristle", VERTICES_PER_BRISTLE );
    setBristlesProgram.setUniform1f( "u_jitter"            , pBristleJitter       );
    setBristlesProgram.setUniform2f( "u_resolution"        , glm::vec2(MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE));
    setBristlesProgram.setUniformTexture( "u_randomsTexture", randomsTexture, 0);


    quadVertexBuffer.draw(GL_TRIANGLE_STRIP, 0, 4);


    setBristlesProgram.end();
    positionsFbo->end();
}

void Brush::project()
{
    projectedPositionsFbo->begin();
    projectProgram.begin();

    projectProgram.setUniformTexture("u_positionsTexture"  , positionsFbo->getTexture(), 0);    
    projectProgram.setUniformTexture("u_velocitiesTexture" , velocitiesFbo->getTexture(), 1);
    projectProgram.setUniformTexture("u_randomsTexture"    , randomsTexture   , 2);
    projectProgram.setUniform1f("u_gravity"           , pGravity              );
    projectProgram.setUniform1f("u_damping"           , pDamping        );
    projectProgram.setUniform1f("u_verticesPerBristle", VERTICES_PER_BRISTLE );
    projectProgram.setUniform2f("u_resolution"        , glm::vec2(MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE));
    
    quadVertexBuffer.draw(GL_TRIANGLE_STRIP, 0, 4);

    projectProgram.end();
    projectedPositionsFbo->end();


}

void Brush::setBristlesProjected()
{
    projectedPositionsFbo->begin();
    setBristlesProgram.begin();


    setBristlesProgram.setUniform3f( "u_brushPosition"     , position             );
    setBristlesProgram.setUniform1f( "u_brushScale"        , pScale                );
    setBristlesProgram.setUniform1f( "u_bristleCount"      , MAX_BRISTLE_COUNT    );
    setBristlesProgram.setUniform1f( "u_bristleLength"     , pBristleLength       );
    setBristlesProgram.setUniform1f( "u_verticesPerBristle", VERTICES_PER_BRISTLE );
    setBristlesProgram.setUniform1f( "u_jitter"            , pBristleJitter       );
    setBristlesProgram.setUniform2f( "u_resolution"        , glm::vec2(MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE));
    setBristlesProgram.setUniformTexture( "u_randomsTexture", randomsTexture, 0);


    quadRowVertexBuffer.draw(GL_TRIANGLE_STRIP, 0, 4);

    setBristlesProgram.end();
    projectedPositionsFbo->end();
}

void Brush::distanceContrain(int pass)
{
    projectedPositionsFboTemp->begin();
    distanceConstraintProgram.begin();

    distanceConstraintProgram.setUniformTexture("u_positionsTexture", projectedPositionsFbo->getTexture(), 0);
    distanceConstraintProgram.setUniform1f("u_pointCount", VERTICES_PER_BRISTLE);
    distanceConstraintProgram.setUniform1f("u_targetDistance", pScale * pBristleLength / (VERTICES_PER_BRISTLE - 1));
    distanceConstraintProgram.setUniform1i("u_pass", pass);
    distanceConstraintProgram.setUniform2f("u_resolution", glm::vec2(MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE));

    quadVertexBuffer.draw(GL_TRIANGLE_STRIP, 0, 4);

    distanceConstraintProgram.end();
    projectedPositionsFboTemp->end();

}

void Brush::blendingContrain(int pass)
{
    projectedPositionsFboTemp->begin();
    bendingConstraintProgram.begin();


    bendingConstraintProgram.setUniformTexture("u_positionsTexture", projectedPositionsFbo->getTexture(), 0);
    bendingConstraintProgram.setUniformTexture("u_randomsTexture", randomsTexture, 1);
    bendingConstraintProgram.setUniform1f("u_pointCount", VERTICES_PER_BRISTLE);
    bendingConstraintProgram.setUniform1f("u_stiffnessVariation", pStiffnessVariation);
    bendingConstraintProgram.setUniform1i("u_pass", pass);
    bendingConstraintProgram.setUniform2f("u_resolution", glm::vec2(MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE));

    quadVertexBuffer.draw(GL_TRIANGLE_STRIP, 0, 4);

    bendingConstraintProgram.end();
    projectedPositionsFboTemp->end();
}

void Brush::planeContrain()
{
    projectedPositionsFboTemp->begin();
    planeConstraintProgram.begin();
    
    planeConstraintProgram.setUniformTexture("u_positionsTexture", projectedPositionsFbo->getTexture(), 0);
    planeConstraintProgram.setUniform2f("u_resolution", glm::vec2( MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE));

    quadVertexBuffer.draw(GL_TRIANGLE_STRIP, 0, 4);

    planeConstraintProgram.end();
    projectedPositionsFboTemp->end();
}

void Brush::updateVelocities()
{
    previousVelocitiesFbo->begin();
    updateVelocityProgram.begin();

    updateVelocityProgram.setUniformTexture("u_positionsTexture", positionsFbo->getTexture(), 0);
    updateVelocityProgram.setUniformTexture("u_projectedPositionsTexture", projectedPositionsFbo->getTexture(), 1);
    updateVelocityProgram.setUniform2f("u_resolution", glm::vec2(MAX_BRISTLE_COUNT, VERTICES_PER_BRISTLE));

    quadVertexBuffer.draw(GL_TRIANGLE_STRIP, 0, 4);

    updateVelocityProgram.end();

    previousVelocitiesFbo->end();
}



void Brush::onMouseDown(ofMouseEventArgs & args)
{
    if (pColorMode == (int)ColorMode::SAMPLE  ){
        sampleShader.update(colorFbo, positionsFbo->getTexture(), colorTexture, pVScale);
    }

    b_painting = true;
}

void Brush::onMouseScrolled(ofMouseEventArgs & args)
{
    pScale += args.scrollY;
    pScale = ofClamp(pScale, pScale.getMin(), pScale.getMax());
}

void Brush::onColorChanged(ofFloatColor & args)
{
    if (pColorMode == (int)ColorMode::SIMPLE_COLOR) {
        colorFbo.begin();
        ofClear(pColor.get());
        colorFbo.end();
    }
}
