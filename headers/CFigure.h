/* 
 * File:   CFigure.h
 * Author: hokan
 *
 * Created on 27 Июнь 2014 г., 18:05
 */

#include "headers/CGameFieldManager.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <ISceneNode.h>
#include <IFileSystem.h>
#include <ISceneNodeFactory.h>
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <IXMLWriter.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

#ifndef CFIGURE_H
#define	CFIGURE_H


// TODO CGameFieldFigure : public CFigure
// in CGameFieldFigure all field-like methods
class CFigure : public irr::scene::ISceneNode {
public:
    CFigure(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* sceneManager, irr::s32 id,
            irr::io::IFileSystem* fileSystem, irr::f32 figureSize = 10.0f, irr::io::path figureFile = "", irr::scene::ESCENE_NODE_TYPE nodeType = irr::scene::ESNT_CUBE);

    bool createFigureFromFile(irr::io::path pathToFile, irr::scene::ESCENE_NODE_TYPE elementalBlockType = irr::scene::ESNT_CUBE);
    bool saveFigureToFile(irr::io::path pathToFile);
    
    bool isFigureEmpty();

    virtual ~CFigure();

    virtual void OnRegisterSceneNode();

    virtual void render();


    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
    

    virtual const irr::core::aabbox3d<irr::f32> getTransformedBoundingBox() const;

    virtual irr::u32 getMaterialCount() const;


    virtual irr::video::SMaterial& getMaterial(irr::u32 num);


    virtual void setPosition(const irr::core::vector3df& newpos);

    virtual const irr::core::vector3df& getPosition() const;
    

    virtual const irr::core::vector3df& getRotation() const;
    
    virtual void setRotation(const irr::core::vector3df& rotation);


    void applyTextureToFigure(const irr::io::path& textureFileName, irr::video::SColor colour);

    irr::f32 getFigureSize() const;

    void setFigureSize(irr::f32 newSize);


    virtual void setScale(const irr::core::vector3df& scale);


    virtual const irr::core::vector3df& getScale() const;
    
    irr::core::vector3di getParentNodeFieldPosition() const;
    
    void constructNodesFieldPositions(irr::core::vector3di parentNodeFieldPosition);  
    void getExtremePositions(irr::core::vector3di direction, irr::core::array<irr::core::vector3di>& extremePositions);
    
    void setParentNodeFieldPosition(irr::core::vector3di newposition);
    
    void affectFieldPositions(irr::core::vector3di affectVector);
    
    void removeAllCreatedTextures();
    void removeAllCreatedTextures(irr::scene::ISceneNode* node);
    

    virtual void remove();

    
    EGF_FIGURE_TYPE getFieldFigureType() const;
    
    // check if figure doesn't have holes (nodes connected side-by-side)
    bool isSolid();       
    
    void removeNodeByOffset(irr::core::vector3di& offset);
    
    irr::core::array<irr::core::vector3di> getFieldPositions() const;
    void setFieldPositions(irr::core::array<irr::core::vector3di> newFieldPositions);   
    
    bool getFieldPositionChanged() const;
    bool setFieldPositionChanged(bool newValue);
    
    irr::core::stringw getFigureName() const;

    // TODO make private
    void removeNodeFromFigure(irr::scene::ISceneNode* nodeToDelete, bool withIntegrityCheck = false);
 irr::scene::ISceneNode* figureParentNode;
 irr::core::array<irr::core::vector3di> fieldPositions;
 irr::core::vector3di parentNodeFieldPosition;
 // find node by offset from root node
    irr::scene::ISceneNode* getNodeByOffset(irr::core::vector3di offset);
    
    bool g();
    irr::scene::ISceneNode* gparent;
    irr::core::vector3di goffset;
private:
    void setCubeColour(irr::scene::ISceneNode* node, irr::video::SColor colour, irr::u32 borderSize = 5);
    // if withIntegrityCheck set to "true", you can't delete nodes that has children, used in editor \
    // cause elements in user created figures must be connected
    
    void setTextureToNodeChildren(irr::scene::ISceneNode* node, irr::video::ITexture* texture, irr::video::SColor colour);
    void recalculateBoundingBox();
    void calculateChildrenBoundingBox(irr::scene::ISceneNode* node, irr::core::aabbox3df& aabb);
    void saveNodeChildrenToFile(irr::io::IXMLWriter* writer, irr::scene::ISceneNode* node);
    bool isSolid(irr::scene::ISceneNode* node);
            
    void construct(irr::scene::ISceneNode* node, irr::core::vector3di nodeFieldPosition);
    
    irr::scene::ISceneNode* getNodeByOffset(irr::scene::ISceneNode* node, irr::core::vector3di offset);

private:
    // not size exactly, but scale factor, cause we create all figures dynamicly via SceneNodeFactory, which
    // doesn't allow us pass external parameters to constructor
    irr::core::vector3df figureSize;
    irr::scene::ESCENE_NODE_TYPE nodeType;
    irr::core::stringw figureName;
    irr::core::stringw comment;    

    irr::core::vector3df boxOffset;
    irr::core::aabbox3df Box;
    irr::video::SMaterial Material;
    irr::io::IFileSystem* fileSystem;
    irr::scene::ISceneNodeFactory* sceneNodeFactory;
   
    irr::video::IVideoDriver* videoDriver;
    bool fieldPositionChanged;
        
};

#endif	/* CFIGURE_H */

