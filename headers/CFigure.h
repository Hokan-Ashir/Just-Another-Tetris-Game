/* 
 * File:   CFigure.h
 * Author: hokan
 *
 * Created on 27 Июнь 2014 г., 18:05
 */

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

class CFigure : public irr::scene::ISceneNode {
public:
    CFigure(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* sceneManager, irr::s32 id,
            irr::io::IFileSystem* fileSystem, irr::f32 figureSize = 10.0f, irr::io::path figureFile = "");

    bool createFigureFromFile(irr::io::path pathToFile);
    bool saveFigureToFile(irr::io::path pathToFile);

    virtual ~CFigure();

    virtual void OnRegisterSceneNode();

    virtual void render();


    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;


    virtual irr::u32 getMaterialCount() const;


    virtual irr::video::SMaterial& getMaterial(irr::u32 num);


    virtual void setPosition(const irr::core::vector3df& newpos);

    void applyTextureToFigure(const irr::io::path& textureFileName, irr::video::SColor colour);

    irr::f32 getFigureSize() const;

    void setFigureSize(irr::f32 newSize);


    virtual void setScale(const irr::core::vector3df& scale);


    virtual const irr::core::vector3df& getScale() const;



private:
    void setCubeColour(irr::scene::ISceneNode* node, irr::video::SColor colour, irr::u32 borderSize = 5);
    // if withIntegrityCheck set to "true", you can't delete nodes that has children, used in editor \
    // cause elements in user created figures must be connected
    void removeNodeFromFigure(irr::scene::ISceneNode* nodeToDelete, bool withIntegrityCheck = false);
    void setTextureToNodeChildren(irr::scene::ISceneNode* node, irr::video::ITexture* texture, irr::video::SColor colour);
    void recalculateBoundingBox();
    void calculateChildrenBoundingBox(irr::scene::ISceneNode* node, irr::core::aabbox3df& aabb);
    void saveNodeChildrenToFile(irr::io::IXMLWriter* writer, irr::scene::ISceneNode* node);

private:
    // not size exactly, but scale factor, cause we create all figures dynamicly via SceneNodeFactory, which
    // doesn't allow us pass external parameters to constructor
    irr::core::vector3df figureSize;
    irr::scene::ESCENE_NODE_TYPE nodeType;
    irr::core::stringw figureName;
    irr::core::stringw comment;

    irr::core::aabbox3d<irr::f32> Box;
    irr::video::SMaterial Material;
    irr::io::IFileSystem* fileSystem;
    irr::scene::ISceneNodeFactory* sceneNodeFactory;
    irr::scene::ISceneNode* figureParentNode;
    irr::video::IVideoDriver* videoDriver;
};

#endif	/* CFIGURE_H */

