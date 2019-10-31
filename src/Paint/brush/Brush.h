#pragma once
#include "ofMain.h"
#include "wbSampleShader.h"

class Brush
{
public:
    Brush();
    ~Brush();

    void setupBrush();

    void init(glm::vec2 pos);
    void setColorTexture(ofTexture & _colorTexture) { colorTexture = _colorTexture; }

    void drawBrush();

    void update(glm::vec2 pos);
    void setupFbo(shared_ptr<ofFbo> fbo, int w, int h, int internalformat);

    ofTexture & getPositionsTexture()          { return positionsFbo->getTexture();          };
    ofTexture & getPreviousPositionsTexture()  { return previousPositionsFbo->getTexture();  };
    ofTexture & getVelocitiesTexture()         { return velocitiesFbo->getTexture();         };
    ofTexture & getPreviousVelocitiesTexture() { return previousVelocitiesFbo->getTexture(); };
    ofTexture & getColorTexture()              { return colorFbo.getTexture(); };

    int   getBristleCount()     const { return MAX_BRISTLE_COUNT;    }
    int   getVertsPerBristle()  const { return VERTICES_PER_BRISTLE; }
    int   getSplatsPerSegment() const { return SPLATS_PER_SEGMENT;   }
    float getSpeed()            const { return speed;  }
    float getScale()            const { return pScale.get(); }
    bool  isPainting()          const { return b_painting || true; }


    ofParameterGroup params;

private:

    enum class ColorMode {
        SIMPLE_COLOR = 0,
        SAMPLE,
        SAMPLE_CONTINIOUS
    };

    static void setupShader(ofShader& shader, const string& vertSource, const string& fragSource);
    void swapTextures(shared_ptr<ofFbo> & fbo0, shared_ptr<ofFbo> & fbo1);


    void setBristles();
    void project();
    void setBristlesProjected();
    void distanceContrain(int pass);
    void blendingContrain(int pass);
    void planeContrain();
    void updateVelocities();

    void onMouseDown(ofMouseEventArgs & args);
    void onMouseScrolled(ofMouseEventArgs & args);
    void onColorChanged(ofFloatColor & args);

    bool b_painting;


    const int   MAX_BRISTLE_COUNT    = 100;
    const int   SPLATS_PER_SEGMENT   = 8;
    const int   VERTICES_PER_BRISTLE = 10;


    ofParameter<float> pBristleLength; 
    ofParameter<float> pBristleJitter;
    ofParameter<int>   pIterations;
    ofParameter<float> pGravity;
    ofParameter<float> pDamping;
    ofParameter<float> pStiffnessVariation;
    ofParameter<float> pHeight;
    ofParameter<float> pScale;
	ofParameter<int>		    pColorMode;
	ofParameter<glm::vec2>	    pVScale;
	ofParameter<ofFloatColor>	pColor;

    ofShader   setBristlesProgram;
    ofShader   projectProgram;
    ofShader   distanceConstraintProgram;
    ofShader   planeConstraintProgram;
    ofShader   bendingConstraintProgram;
    ofShader   updateVelocityProgram;

    ofShader   brushProgram;

    shared_ptr<ofFbo>   positionsFbo;
    shared_ptr<ofFbo>   previousPositionsFbo;
    shared_ptr<ofFbo>   velocitiesFbo;
    shared_ptr<ofFbo>   previousVelocitiesFbo;
    shared_ptr<ofFbo>   projectedPositionsFbo;
    shared_ptr<ofFbo>   projectedPositionsFboTemp;
    shared_ptr<ofFbo>   debugFbo;

    ofTexture   randomsTexture;

    ofVbo brushBuffer;

    ofVbo quadVertexBuffer;
    ofVbo quadRowVertexBuffer;

    ofTexture temp;

    flowTools::wbSampleShader sampleShader;
    ofTexture      colorTexture;
    ofFbo          colorFbo;

    int indexCount;

    glm::vec3 position;
    float speed;
    


};

