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

/**
 * Class of figure that represents some structure (square, t-block, l-block, line, etc.),
 * composed from "elemental blocks" (squares or spheres, for now) - ISceneNodes.
 * Also this class contains methods for game field interaction (field positions, border field positions etc.)
 */
class CFigure : public irr::scene::ISceneNode {
public:
    /**
     * Constructor
     * 
     * parameters required for ISceneNode constructor
     * @param parent - parent scene node for figure
     * @param sceneManager - scene manager of scene 
     * @param id - id of current scene node
     * 
     * CFigure parameters
     * @param fileSystem - pointer to file system, required for saving/loading from files
     * @param figureSize - scale factor for every side of figure
     * @param figureFile - path (relative or full) to figure file
     * @param nodeType - node type which will be "elemental" block for figures, i.e. ESNT_SPHERE creates figure composed from spheres
     */
    CFigure(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* sceneManager, irr::s32 id,
            irr::io::IFileSystem* fileSystem, irr::f32 figureSize = 10.0f, irr::io::path figureFile = "", irr::scene::ESCENE_NODE_TYPE nodeType = irr::scene::ESNT_CUBE);

    /**
     * Creates figure from xml file, which represens structure of figure
     * 
     * i.e:
     * <?xml version="1.0"?>
        <figure name="tblock" comment="typical tblock node">
        <side type="down">
        </side>
        <side type="left">
        </side>
        <side type="right">
        </side>
        </figure>
     * this file represents t-block figure:
     * X-*-X
     *   |
     *   X
     * where node marked as * is root node (corresponds to figure-tag), and nodes marked as X is child nodes (corresponds to side-tags)
     * 
     * @param pathToFile - path (relative or full) to figure file
     * @param elementalBlockType - node type which will be "elemental" block for figures, i.e. ESNT_SPHERE creates figure composed from spheres
     * @return true if figure created, false otherwise
     */
    bool createFigureFromFile(irr::io::path pathToFile, irr::scene::ESCENE_NODE_TYPE elementalBlockType = irr::scene::ESNT_CUBE);

    /**
     * Saves this figure-class instance to file
     * 
     * @param pathToFile - path (relative or full) to figure file
     * @return true if figure saved to file, false otherwise
     */
    bool saveFigureToFile(irr::io::path pathToFile);

    /**
     * Checks if figure has no nodes instances, a.k.a. elemental blocks
     * 
     * @return true if figure has no elemental blocks, false otherwise
     */
    bool isFigureEmpty();

    /**
     * Destructor
     * 
     * remove all created elemental blocks, attached animators and created textures
     */
    virtual ~CFigure();

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual void OnRegisterSceneNode();

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual void render();

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual const irr::core::aabbox3d<irr::f32> getTransformedBoundingBox() const;

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual irr::u32 getMaterialCount() const;

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual irr::video::SMaterial& getMaterial(irr::u32 num);

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual void setPosition(const irr::core::vector3df& newpos);

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual const irr::core::vector3df& getPosition() const;

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual const irr::core::vector3df& getRotation() const;

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual void setRotation(const irr::core::vector3df& rotation);

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual void setScale(const irr::core::vector3df& scale);

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual const irr::core::vector3df& getScale() const;

    /**
     * Removes node, also remove any created textures, subnodes and deattach animators
     */
    virtual void remove();

    /**
     * Creates a copy from texrure, then paint it with specific colour (exclude borders see setCubeColour method)
     * 
     * @param textureFileName - path (relative or full) to texture file
     * @param colour - colour to paint texture exclude borders
     */
    void applyTextureToFigure(const irr::io::path& textureFileName, irr::video::SColor colour);

    /**
     * Getter
     * 
     * @return figure scale factor by one axis
     */
    irr::f32 getFigureSize() const;

    /**
     * Setter
     * 
     * @param newSize - new figure scale factor, applies to all axises
     */
    void setFigureSize(irr::f32 newSize);

    /**
     * Getter
     * 
     * @return figure parent node position on game field
     */
    irr::core::vector3di getParentNodeFieldPosition() const;

    /**
     * Based on figure parent node game field position and figure structure constructs all figure nodes position on field
     * i.e. t-block 
     * X-*-X
     *   |
     *   X
     * with parent node (marked as *) game field position 5,5,5 and rotation 0,0,0 will have
     * 5,4,5
     * 4,5,5
     * 5,5,5
     * 5,5,6
     * positions in array
     * 
     * NOTE: result array of game field positions is sorted
     * @param parentNodeFieldPosition - parent node game field position
     */
    void constructNodesFieldPositions(irr::core::vector3di parentNodeFieldPosition);
    
    /**
     * Retrieves border figure positions on game field in specific direction
     * @see CMoveDownAnimator
     * i.e. t-block 
     * X-*-X
     *   |
     *   X
     * with parent node (marked as *) game field position 5,5,5 and rotation 0,0,0 with
     * 5,4,5
     * 4,5,5
     * 5,5,5
     * 5,5,6
     * game field positions and 0,-1,0 direction vector will have
     * 5,4,5
     * 4,5,5
     * 5,5,6
     * exreme game field positions
     * 
     * @param direction
     * @param extremePositions
     */
    void getExtremePositions(irr::core::vector3di direction, irr::core::array<irr::core::vector3di>& extremePositions);

    /**
     * Setter
     * 
     * @param newposition - new parent node position on game field
     */
    void setParentNodeFieldPosition(irr::core::vector3di newposition);

    /**
     * Adds to all field positions affect vector
     * 
     * @param affectVector - vector to add to all field positions
     */
    void affectFieldPositions(irr::core::vector3di affectVector);

    /**
     * Remove all created textures from figure 
     */
    void removeAllCreatedTextures();
    
    /**
     * Recursive version of removeAllCreatedTextures method
     * 
     * @param node - pointer which texture and all its children textures to remove
     */
    void removeAllCreatedTextures(irr::scene::ISceneNode* node);


    /**
     * Getter
     * 
     * @return game field figure type
     */
    EGF_FIGURE_TYPE getFieldFigureType() const;
    
    /**
     * Check if figure doesn't have holes (nodes connected side-by-side)
     * 
     * @return true, if figure has no holes, false otherwise
     */
    bool isSolid();

    /**
     * Removes node from figure by offset (full relative position from root node)
     * 
     * @param offset
     */
    void removeNodeByOffset(irr::core::vector3di& offset);

    /**
     * Getter
     * 
     * @return array of figure's game field positions
     */
    irr::core::array<irr::core::vector3di> getFieldPositions() const;
    
    /**
     * Setter
     * 
     * @param newFieldPositions - new array of figure's game field positions
     */
    void setFieldPositions(irr::core::array<irr::core::vector3di> newFieldPositions);

    /**
     * Getter
     * @see CMoveDownAnimator
     * 
     * @return true, if figure change its position on game field
     */
    bool getFieldPositionChanged() const;
    
    /**
     * Setter
     * @see CMoveDownAnimator
     * 
     * @param newValue - new value, if figure change its position on game field
     */
    void setFieldPositionChanged(bool newValue);

    /**
     * Getter
     * 
     * @return figure name
     */
    irr::core::stringw getFigureName() const;

    // TODO make private    
    /**
     * array of figure game field positions
     */
    irr::core::array<irr::core::vector3di> fieldPositions;
    
    /**
     * vector to parent node game field position
     */
    irr::core::vector3di parentNodeFieldPosition;                

    /**
     * If user constructs line (or plane) on game field that includes some node in the middle of figure
     * this function calculates figure collapse and indicates CMoveDownAnimator that it's already collapsed
     * so it can change collapsed figure nodes positions on game field
     * i.e.
     * t-block
     * 1 X
     *   |
     * 2 X---*     
     *   |
     * 3  X
     * user constructs line #2, so figure has hole, this function collide figure, so it will look like
     * X
     * |
     * X
     * @see CMoveDownAnimator
     * 
     * @return true, if figure already collapsed and animator need to recalculate nodes positions, false otherwise
     */
    bool isFigureCollapse();
private:
    /**
     * Finds node by offset (full relative position) from root node
     * 
     * @param offset - vector of full relative position from root node
     * @return pointer to node inside figure with offset, NULL otherwise
     */
    irr::scene::ISceneNode* getNodeByOffset(irr::core::vector3di offset);
    
    /**
     * Sets elemental block colour (get its texture, paint with colour, exclude border pixels from every side)
     * 
     * @param node - node to paint
     * @param colour - colour to use
     * @param borderSize - how many pixels of texture exclude from painting from every side
     */
    void setElementalBlockColour(irr::scene::ISceneNode* node, irr::video::SColor colour, irr::u32 borderSize = 5);
        
    /**
     * Remove node from figure. Select new parent, recalculate relative node positions, bounding box and figure game field positions if necessary
     * 
     * @param nodeToDelete - pointer to node to delete from figure
     * @param withIntegrityCheck - if withIntegrityCheck set to "true", you can't delete nodes that has children, used in editor \
     * cause elements in user created figures must be connected
     */
    void removeNodeFromFigure(irr::scene::ISceneNode* nodeToDelete, bool withIntegrityCheck = false);
    
    /**
     * Recursively sets texture to children nodes (see applyTextureToFigure method)
     * 
     * @param node - node and its children which will be affected
     * @param texture - texture to attach to children
     * @param colour - colour with which texture will be painted
     */
    void setTextureToNodeChildren(irr::scene::ISceneNode* node, irr::video::ITexture* texture, irr::video::SColor colour);
    
    /**
     * Simply recalculates bounding box
     */
    void recalculateBoundingBox();
    
    /**
     * Recursively calculate bounding box
     * 
     * @param node - node and its children which will be used
     * @param aabb - result axis aligned bounding box
     */
    void calculateChildrenBoundingBox(irr::scene::ISceneNode* node, irr::core::aabbox3df& aabb);
    
    /**
     * Recursively save nodes to file
     * 
     * @param writer - pointer to xml writer instance
     * @param node - node and its children which will be used
     */
    void saveNodeChildrenToFile(irr::io::IXMLWriter* writer, irr::scene::ISceneNode* node);
    
    /**
     * Recursively check is nodes is solid (see isSolid method)
     * 
     * @param node - node and its children which will be used
     * @return true if nodes connected without holes, false otherwise
     */
    bool isSolid(irr::scene::ISceneNode* node);

    /**
     * Recursively construct nodes game field position (see constructNodesFieldPositions method)
     * 
     * @param node - node which children which will be used
     * @param nodeFieldPosition - node game field position
     */
    void construct(irr::scene::ISceneNode* node, irr::core::vector3di nodeFieldPosition);

    /**
     * Recursively search for node in figure by offset (see getNodeByOffset method)
     * 
     * @param node - node and its children which will be used
     * @param offset - full relative path to searching node
     * @return pointer to node if found, NULL otherwise
     */
    irr::scene::ISceneNode* getNodeByOffset(irr::scene::ISceneNode* node, irr::core::vector3di offset);

    /**
     * Recursively collapse nodes (see isFigureCollapse method)
     * 
     * @param node - node and its children which will be used
     * @param vector - vector which will be subtracted from all node children recursively
     */
    void collapseNodes(irr::scene::ISceneNode* node, irr::core::vector3di vector);

private:
    /**
     * not size exactly, but scale factor, cause we create all figures dynamicly via SceneNodeFactory, which \
     * doesn't allow us pass external parameters to constructor
     */
    irr::core::vector3df figureSize;
    
    /**
     * figure elemental blocks type on game field
     */
    irr::scene::ESCENE_NODE_TYPE nodeType;
    
    /**
     * figure name
     */
    irr::core::stringw figureName;
    
    /**
     * comment
     */
    irr::core::stringw comment;

    /**
     * axis aligned bounding box offset (appears when figure change parent node, see removeNodeFromFigure method)
     */
    irr::core::vector3df boxOffset;
    
    /**
     * axis aligned bounding box
     */
    irr::core::aabbox3df Box;
    
    /**
     * figure material
     */
    irr::video::SMaterial Material;
    
    /**
     * Irrlicht engine file system
     */
    irr::io::IFileSystem* fileSystem;
    
    /**
     * node factory that is used to produce new elemental blocks for figures
     */
    irr::scene::ISceneNodeFactory* sceneNodeFactory;

    /**
     * Irrlicht video driver
     */
    irr::video::IVideoDriver* videoDriver;
    
    /**
     * flag that figure change its position on game field
     */
    bool fieldPositionChanged;
    
    /**
     * parent node of whole figure
     */
    irr::scene::ISceneNode* figureParentNode;
    
    /**
     * contains last found collapsable node. Node which (and which children)
     * must be affected by collision offset
     */
    irr::scene::ISceneNode* collapsableNode;
};

#endif	/* CFIGURE_H */

