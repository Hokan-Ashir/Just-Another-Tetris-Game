/* 
 * File:   CFigure.cpp
 * Author: hokan
 * 
 * Created on 27 Июнь 2014 г., 18:05
 */

#include "headers/CFigure.h"
#include <iostream>

// TODO method with callback function for child-iteration cases

CFigure::CFigure(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* sceneManager, irr::s32 id,
        irr::io::IFileSystem* fileSystem, irr::f32 figureSize, irr::io::path figureFile, irr::scene::ESCENE_NODE_TYPE nodeType) :
irr::scene::ISceneNode(parent, sceneManager, id), figureName(L""), comment(L""), fileSystem(fileSystem), figureParentNode(NULL) {
#ifdef _DEBUG
    setDebugName("CFigure");
#endif

    sceneNodeFactory = sceneManager->getSceneNodeFactory(0);
    videoDriver = sceneManager->getVideoDriver();
    this->figureSize = irr::core::vector3df(figureSize, figureSize, figureSize);

    if (figureFile == "" || !createFigureFromFile(figureFile, nodeType)) {
        // hope we get default sceneManager with default sceneNodeFactory which has method to create default cube sceneNode
        figureParentNode = sceneNodeFactory->addSceneNode(irr::scene::ESNT_CUBE, getParent());
        figureParentNode->setScale(this->figureSize);
        Box = figureParentNode->getBoundingBox();
        nodeType = irr::scene::ESNT_CUBE;
    }

    boxOffset.set(0.0f, 0.0f, 0.0f);
    fieldPositionChanged = false;
    gparent = NULL;
}

CFigure::~CFigure() {
}

// TODO closing control

bool CFigure::createFigureFromFile(irr::io::path pathToFile, irr::scene::ESCENE_NODE_TYPE elementalBlockType) {
    // clear all children and parent node itself
    if (figureParentNode != NULL) {
        if (figureParentNode->getChildren().size() != 0) {
            figureParentNode->removeAll();
        }
        figureParentNode->remove();
    }

    irr::io::IXMLReader* xmlReader = fileSystem->createXMLReader(pathToFile);
    if (!xmlReader) {
        return false;
    }

    const irr::core::stringw sideTag(L"side");
    irr::core::stringw currentSection;
    const irr::core::stringw figureTag(L"figure");

    nodeType = elementalBlockType;

    irr::scene::ISceneNode* currentParentNode = NULL;
    //while there is more to read
    while (xmlReader->read()) {
        //check the node type
        switch (xmlReader->getNodeType()) {
            case irr::io::EXN_ELEMENT:
            {
                if (currentSection.empty() && figureTag.equals_ignore_case(xmlReader->getNodeName())) {
                    currentSection = figureTag;

                    // we don't care about empty name or comment, cause it's not so critical
                    irr::core::stringw nameAttributeValue = xmlReader->getAttributeValueSafe(L"name");
                    figureName = nameAttributeValue;

                    irr::core::stringw commentAttributeValue = xmlReader->getAttributeValueSafe(L"comment");
                    comment = commentAttributeValue;

                    irr::scene::ISceneNode* node = sceneNodeFactory->addSceneNode(nodeType, getParent());
                    node->setScale(figureSize);
                    figureParentNode = node;
                    currentParentNode = node;
                } else if (currentSection.equals_ignore_case(figureTag) && sideTag.equals_ignore_case(xmlReader->getNodeName())) {
                    irr::core::stringw typeAttributeValue = xmlReader->getAttributeValueSafe(L"type");
                    if (!typeAttributeValue.empty()
                            && typeAttributeValue.find(irr::core::stringw(L"updownleftrightfrontback").c_str())) {
                        irr::scene::ISceneNode* node = sceneNodeFactory->addSceneNode(nodeType, getParent());
                        // cause node is symmetric by all sides we can get any coordinate
                        irr::f32 nodeSize = (node->getBoundingBox().MaxEdge.X - node->getBoundingBox().MinEdge.X);

                        node->setParent(currentParentNode);
                        if (typeAttributeValue.equals_ignore_case(irr::core::stringw(L"up"))) {
                            node->setPosition(irr::core::vector3df(0.0f, nodeSize, 0.0f));
                        } else if (typeAttributeValue.equals_ignore_case(irr::core::stringw(L"down"))) {
                            node->setPosition(irr::core::vector3df(0.0f, -nodeSize, 0.0f));
                        } else if (typeAttributeValue.equals_ignore_case(irr::core::stringw(L"right"))) {
                            node->setPosition(irr::core::vector3df(nodeSize, 0.0f, 0.0f));
                        } else if (typeAttributeValue.equals_ignore_case(irr::core::stringw(L"left"))) {
                            node->setPosition(irr::core::vector3df(-nodeSize, 0.0f, 0.0f));
                        } else if (typeAttributeValue.equals_ignore_case(irr::core::stringw(L"front"))) {
                            node->setPosition(irr::core::vector3df(0.0f, 0.0f, nodeSize));
                        } else if (typeAttributeValue.equals_ignore_case(irr::core::stringw(L"back"))) {
                            node->setPosition(irr::core::vector3df(0.0f, 0.0f, -nodeSize));
                        }
                        currentParentNode = node;
                    }
                }
            }
                break;

            case irr::io::EXN_ELEMENT_END:
                currentParentNode = currentParentNode->getParent();
                break;
        }
    }

    // don't forget to delete the xml reader
    xmlReader->drop();

    // file is invalid and we read nothing
    if (currentSection.empty()) {
        // TODO make notification
        return false;
    }

    recalculateBoundingBox();

    return true;
}

void CFigure::saveNodeChildrenToFile(irr::io::IXMLWriter* writer, irr::scene::ISceneNode* node) {
    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        irr::core::stringw direction;
        if ((*it)->getPosition().X == -figureSize.X) {
            direction = L"left";
        } else if ((*it)->getPosition().X == figureSize.X) {
            direction = L"right";
        } else if ((*it)->getPosition().Y == figureSize.Y) {
            direction = L"up";
        } else if ((*it)->getPosition().Y == -figureSize.Y) {
            direction = L"down";
        } else if ((*it)->getPosition().X == figureSize.Z) {
            direction = L"front";
        } else if ((*it)->getPosition().X == -figureSize.Z) {
            direction = L"back";
        }
        writer->writeElement(L"side", false, L"type", direction.c_str());
        saveNodeChildrenToFile(writer, (*it));
        writer->writeClosingTag(L"side");
    }
}

// TODO closing control

bool CFigure::saveFigureToFile(irr::io::path pathToFile) {
    if (!isSolid()) {
        // TODO make notification
        return false;
    }

    irr::io::IXMLWriter* xwriter = fileSystem->createXMLWriter(pathToFile);
    if (!xwriter) {
        irr::io::IWriteFile* file = fileSystem->createAndWriteFile(pathToFile);
        if (!file) {
            return false;
        }

        xwriter = fileSystem->createXMLWriter(file);
        if (!xwriter) {
            return false;
        }
    }

    //write out the obligatory xml header. Each xml-file needs to have exactly one of those.
    xwriter->writeXMLHeader();

    xwriter->writeElement(L"figure", false, L"name", figureName.c_str(), L"comment", comment.c_str());
    xwriter->writeLineBreak();

    saveNodeChildrenToFile(xwriter, figureParentNode);

    xwriter->writeClosingTag(L"figure");

    return true;
}

bool CFigure::isFigureEmpty() {
    return (figureParentNode == NULL);
}

void CFigure::recalculateBoundingBox() {
    calculateChildrenBoundingBox(figureParentNode, Box);
    boxOffset.set(0.0f, 0.0f, 0.0f);
}

// easy way

/*

 |
 V
 0-0
   |
   0-0-0
       ^
       |
 
calculate AABB around nodes without children, then combine in the same way 
 */
void CFigure::calculateChildrenBoundingBox(irr::scene::ISceneNode* node, irr::core::aabbox3df& aabb) {
    irr::core::vector3df minEdge = node->getBoundingBox().MinEdge;
    irr::core::vector3df maxEdge = node->getBoundingBox().MaxEdge;

    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    // construct bounding box
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        irr::core::vector3df childMinEdge = ((*it)->getBoundingBox().MinEdge + (*it)->getPosition());
        irr::core::vector3df childMaxEdge = ((*it)->getBoundingBox().MaxEdge + (*it)->getPosition());

        // we have to check ge/le-statements by coordinate, cause default vector3d operators
        // check them in alphabetical order
        if (childMinEdge.X < minEdge.X) {
            minEdge.X = childMinEdge.X;
        }
        if (childMinEdge.Y < minEdge.Y) {
            minEdge.Y = childMinEdge.Y;
        }
        if (childMinEdge.Z < minEdge.Z) {
            minEdge.Z = childMinEdge.Z;
        }

        if (childMaxEdge.X > maxEdge.X) {
            maxEdge.X = childMaxEdge.X;
        }
        if (childMaxEdge.Y > maxEdge.Y) {
            maxEdge.Y = childMaxEdge.Y;
        }
        if (childMaxEdge.Z > maxEdge.Z) {
            maxEdge.Z = childMaxEdge.Z;
        }

        irr::core::aabbox3df childAABB;
        calculateChildrenBoundingBox(*it, childAABB);
        childAABB.MaxEdge += (*it)->getPosition();
        childAABB.MinEdge += (*it)->getPosition();

        if (childAABB.MinEdge.X < minEdge.X) {
            minEdge.X = childAABB.MinEdge.X;
        }
        if (childAABB.MinEdge.Y < minEdge.Y) {
            minEdge.Y = childAABB.MinEdge.Y;
        }
        if (childAABB.MinEdge.Z < minEdge.Z) {
            minEdge.Z = childAABB.MinEdge.Z;
        }

        if (childAABB.MaxEdge.X > maxEdge.X) {
            maxEdge.X = childAABB.MaxEdge.X;
        }
        if (childAABB.MaxEdge.Y > maxEdge.Y) {
            maxEdge.Y = childAABB.MaxEdge.Y;
        }
        if (childAABB.MaxEdge.Z > maxEdge.Z) {
            maxEdge.Z = childAABB.MaxEdge.Z;
        }
    }

    aabb.MaxEdge = maxEdge;
    aabb.MinEdge = minEdge;
}

EGF_FIGURE_TYPE CFigure::getFieldFigureType() const {
    if (nodeType == irr::scene::ESNT_CUBE) {
        return EGF_CUBE;
    } else if (nodeType == irr::scene::ESNT_SPHERE) {
        return EGF_SPHERE;
    } else {
        return EGF_EMPTY;
    }
}

bool CFigure::isSolid(irr::scene::ISceneNode* node) {
    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        irr::core::vector3df position = (*it)->getPosition();
        if (position.getLengthSQ() > (figureSize.X * figureSize.X)) {
            return false;
        }
        if (!isSolid(*it)) {
            return false;
        }
    }
    return true;
}

bool CFigure::isSolid() {
    return isSolid(figureParentNode);
}

irr::core::vector3di CFigure::getParentNodeFieldPosition() const {
    return parentNodeFieldPosition;
}

void CFigure::setParentNodeFieldPosition(irr::core::vector3di newposition) {
    parentNodeFieldPosition = newposition;
    constructNodesFieldPositions(newposition);
    fieldPositionChanged = true;
}

bool CFigure::getFieldPositionChanged() const {
    return fieldPositionChanged;
}

bool CFigure::setFieldPositionChanged(bool newValue) {
    fieldPositionChanged = newValue;
}

void CFigure::affectFieldPositions(irr::core::vector3di affectVector) {
    for (irr::u32 i = 0; i < fieldPositions.size(); ++i) {
        fieldPositions[i] += affectVector;
    }
}

void CFigure::construct(irr::scene::ISceneNode* node, irr::core::vector3di nodeFieldPosition) {
    fieldPositions.push_back(nodeFieldPosition);

    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        // calculate child translation offset        
        irr::core::vector3di childFieldPosition;
        irr::core::vector3di tempVector;

        if (getRotation() == irr::core::vector3df()) {
            childFieldPosition.X = ((*it)->getPosition() / figureSize).X;
            childFieldPosition.Y = ((*it)->getPosition() / figureSize).Y;
            childFieldPosition.Z = ((*it)->getPosition() / figureSize).Z;
        } else {
            // calculate child rotation offset
            if (getRotation().X != 0) {
                tempVector.X = 0;
                tempVector.Y = ((*it)->getPosition() / figureSize).Y;
                tempVector.Z = 0;
                tempVector.rotateYZBy(getRotation().X, irr::core::vector3di(0, 0, 0));
                childFieldPosition += tempVector;

                tempVector.X = 0;
                tempVector.Y = 0;
                tempVector.Z = ((*it)->getPosition() / figureSize).Z;
                tempVector.rotateYZBy(getRotation().X, irr::core::vector3di(0, 0, 0));
                childFieldPosition += tempVector;
            }

            if (getRotation().Y != 0) {
                tempVector.X = ((*it)->getPosition() / figureSize).X;
                tempVector.Y = 0;
                tempVector.Z = 0;
                tempVector.rotateXZBy(getRotation().Y, irr::core::vector3di(0, 0, 0));
                childFieldPosition += tempVector;

                tempVector.X = 0;
                tempVector.Y = 0;
                tempVector.Z = ((*it)->getPosition() / figureSize).Z;
                tempVector.rotateXZBy(getRotation().Y, irr::core::vector3di(0, 0, 0));
                childFieldPosition += tempVector;
            }

            if (getRotation().Z != 0) {
                tempVector.X = ((*it)->getPosition() / figureSize).X;
                tempVector.Y = 0;
                tempVector.Z = 0;
                tempVector.rotateXYBy(getRotation().Z, irr::core::vector3di(0, 0, 0));
                childFieldPosition += tempVector;

                tempVector.X = 0;
                tempVector.Y = ((*it)->getPosition() / figureSize).Y;
                tempVector.Z = 0;
                tempVector.rotateXYBy(getRotation().Z, irr::core::vector3di(0, 0, 0));
                childFieldPosition += tempVector;
            }
        }
        std::cout << "childFieldPosition. " << childFieldPosition.X << ", " << childFieldPosition.Y << ", " << childFieldPosition.Z << std::endl;
        childFieldPosition += nodeFieldPosition;

        construct(*it, childFieldPosition);
    }
}

// simplier way

/*
 construct list<vector3di> fieldPositions of _every_ node, every time figure reconstructs
 * figure change root node position on field
 * figure loses its nodes
 
 run through it if you want to get extreme positions 
 */

void CFigure::constructNodesFieldPositions(irr::core::vector3di parentNodeFieldPosition) {
    if (fieldPositions.size() != 0) {
        fieldPositions.clear();
    }
    construct(figureParentNode, parentNodeFieldPosition);
    // TODO rewrite helper
    // we need to sort fieldPositions array, cause it's useful in CMoveDownAnimator:
    /*
     example: moving down. Root node, marked as * has field coordinates 5,5,5
     0-x
       |
     *-0-0
     in this case x has coordinates 5,6,5. So if we sort values we have
     5,5,5
     5,6,5
     ...
     in such way, when we iterate reversevly through fieldPositions, we move nodes from bottom to top
     and field fills correctly
     When we move figure up, we have to iterate forward so field fills from top nodes to bottom
     
     So, if we move in "-value" direction, we use reverse iterators, if in "value" direction, we use straignt iterators
     */
    fieldPositions.sort();
}

void CFigure::getExtremePositions(irr::core::vector3di direction, irr::core::array<irr::core::vector3di>& extremePositions) {
    extremePositions.push_back(fieldPositions[0]);
    for (irr::u32 i = 1; i < fieldPositions.size(); ++i) {
        bool newPositionFound = false;
        for (irr::u32 j = 0; j < extremePositions.size(); ++j) {
            // sift unnecessary nodes
            if (// Movement in X axis. Check if other nodes don't have same Y and Z coordinates
                    (direction.X != 0 && (fieldPositions[i].Y != extremePositions[j].Y || fieldPositions[i].Z != extremePositions[j].Z)) ||
                    // Movement in Y axis. Check if other nodes don't have same X and Z coordinates
                    (direction.Y != 0 && (fieldPositions[i].X != extremePositions[j].X || fieldPositions[i].Z != extremePositions[j].Z)) ||
                    // Movement in Z axis. Check if other nodes don't have same X and Y coordinates
                    (direction.Z != 0 && (fieldPositions[i].X != extremePositions[j].X || fieldPositions[i].Y != extremePositions[j].Y))) {
                continue;
            }


            // cause we try to find most right/up/down/etc. nodes, one if-statement is all we need
            if (// movement in left direction, seek for more left position
                    (direction.X < 0 && (fieldPositions[i].X < extremePositions[j].X)) ||
                    // movement in right direction, seek for more right position
                    (direction.X > 0 && (fieldPositions[i].X > extremePositions[j].X)) ||
                    // movement in down direction, seek ----//-----
                    (direction.Y < 0 && (fieldPositions[i].Y < extremePositions[j].Y)) ||
                    // movement in up direction
                    (direction.Y > 0 && (fieldPositions[i].Y > extremePositions[j].Y)) ||
                    // movement in back direction
                    (direction.Z < 0 && (fieldPositions[i].Z < extremePositions[j].Z)) ||
                    // movement in front direction
                    (direction.Z > 0 && (fieldPositions[i].Z > extremePositions[j].Z))) {
                extremePositions[j] = fieldPositions[i];
            }
            newPositionFound = true;
            break;
        }

        // found nothing in extreme positions array
        if (!newPositionFound) {
            extremePositions.push_back(fieldPositions[i]);
        }
    }
}

void CFigure::OnRegisterSceneNode() {
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void CFigure::render() {
    videoDriver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
    if (DebugDataVisible) {
        irr::video::SMaterial m;
        m.Lighting = false;
        m.AntiAliasing = 0;
        videoDriver->setMaterial(m);

        if (DebugDataVisible & irr::scene::EDS_BBOX) {
            videoDriver->draw3DBox(getTransformedBoundingBox(), irr::video::SColor(255, 255, 255, 255));
        }
    }
}

void CFigure::setPosition(const irr::core::vector3df& newpos) {
    figureParentNode->setPosition(newpos);
}

const irr::core::vector3df& CFigure::getPosition() const {
    return figureParentNode->getPosition();
}

irr::core::array<irr::core::vector3di> CFigure::getFieldPositions() const {
    return fieldPositions;
}

void CFigure::setFieldPositions(irr::core::array<irr::core::vector3di> newFieldPositions) {
    fieldPositions = newFieldPositions;
}

const irr::core::aabbox3d<irr::f32>& CFigure::getBoundingBox() const {
    return Box;
}

const irr::core::aabbox3d<irr::f32> CFigure::getTransformedBoundingBox() const {
    irr::core::aabbox3df box = Box;
    irr::core::matrix4 transformationMatrix = figureParentNode->getRelativeTransformation();
    transformationMatrix.transformBoxEx(box);
    box.MinEdge += boxOffset;
    box.MaxEdge += boxOffset;
    return box;
}

irr::u32 CFigure::getMaterialCount() const {
    return 1;
}

irr::video::SMaterial& CFigure::getMaterial(irr::u32 num) {
    return Material;
}

void CFigure::removeAllCreatedTextures(irr::scene::ISceneNode* node) {
    irr::video::ITexture* texture = node->getMaterial(0).getTexture(0);
    if (texture != NULL) {
        videoDriver->removeTexture(texture);
    }

    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        removeAllCreatedTextures(*it);
    }
}

void CFigure::removeAllCreatedTextures() {
    removeAllCreatedTextures(figureParentNode);
}

void CFigure::remove() {
    if (figureParentNode != NULL) {
        removeAllCreatedTextures();
        irr::scene::ISceneNodeAnimatorList animators = figureParentNode->getAnimators();
        for(irr::scene::ISceneNodeAnimatorList::Iterator it = animators.begin(); it != animators.end(); ++it) {
            (*it)->drop();
        }
        figureParentNode->removeAll();
        figureParentNode->remove();
    }

    if (Parent) {
        Parent->removeChild(this);
    }
}

// recursevly

void CFigure::setTextureToNodeChildren(irr::scene::ISceneNode* node, irr::video::ITexture* texture, irr::video::SColor colour) {
    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        (*it)->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        (*it)->setMaterialTexture(0, texture);
        setCubeColour(*it, colour);
        setTextureToNodeChildren(*it, texture, colour);
    }
}

void CFigure::applyTextureToFigure(const irr::io::path& textureFileName, irr::video::SColor colour) {
    // TODO check this stuff
    irr::video::ITexture* texture = videoDriver->getTexture(textureFileName);
    irr::video::ITexture* newTexture = videoDriver->addTexture(irr::core::dimension2du(texture->getSize().Width, texture->getSize().Height), "newTexture", texture->getColorFormat());
    irr::video::ITexture* newTexture2 = videoDriver->addTexture(irr::core::dimension2du(texture->getSize().Width, texture->getSize().Height), "newTexture2", texture->getColorFormat());
    irr::video::SColor* oldTexturePixels = (irr::video::SColor*)texture->lock(irr::video::ETLM_READ_ONLY);
    irr::video::SColor* newTexturePixels = (irr::video::SColor*)newTexture->lock(irr::video::ETLM_WRITE_ONLY);
    irr::video::SColor* newTexturePixels2 = (irr::video::SColor*)newTexture2->lock(irr::video::ETLM_WRITE_ONLY);
    for (irr::u32 i = 0; i < texture->getSize().Width; ++i) {
        for (irr::u32 j = 0; j < texture->getSize().Height; ++j) {
            newTexturePixels[i * texture->getSize().Width + j] = oldTexturePixels[i * texture->getSize().Width + j];
            newTexturePixels2[i * texture->getSize().Width + j] = oldTexturePixels[i * texture->getSize().Width + j];
        }
    }
    texture->unlock();
    newTexture->unlock();
    newTexture2->unlock();

    figureParentNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    figureParentNode->setMaterialTexture(0, newTexture2);
    setCubeColour(figureParentNode, irr::video::SColor(255, 255, 255, 100));
    setTextureToNodeChildren(figureParentNode, newTexture, colour);

}

// because figureSize is vector with similar values, we can obtain any of them

irr::f32 CFigure::getFigureSize() const {
    return figureSize.X;
}

void CFigure::setFigureSize(irr::f32 newSize) {
    figureSize.X = newSize;
    figureSize.Y = newSize;
    figureSize.Z = newSize;
}

void CFigure::setScale(const irr::core::vector3df& scale) {
    RelativeScale = scale;
    if (figureParentNode != NULL) {
        figureParentNode->setScale(scale);
    }
}

const irr::core::vector3df& CFigure::getScale() const {
    if (figureParentNode != NULL) {
        return figureParentNode->getScale();
    } else {
        return RelativeScale;
    }
}

const irr::core::vector3df& CFigure::getRotation() const {
    if (figureParentNode != NULL) {
        return figureParentNode->getRotation();
    } else {
        return RelativeRotation;
    }
}

void CFigure::setRotation(const irr::core::vector3df& rotation) {
    RelativeRotation = rotation;
    if (figureParentNode != NULL) {
        figureParentNode->setRotation(rotation);
    }
}

irr::core::stringw CFigure::getFigureName() const {
    return figureName;
}

// here can be euristic:

/*
 0-0
   |
   0-0-*
   |
   0-0-0
 if we want to find node marked as *, we can clip subtrees of incorrect solutions (like in branch-&-bound algorithm)
 ONLY IF we have "reasonable" figures like in fig. up. But because user may construct any figures, like:
 0-0
   |
   0   *
   |   |
   0-0-0
 we must perform full search
 */
irr::scene::ISceneNode* CFigure::getNodeByOffset(irr::scene::ISceneNode* node, irr::core::vector3di offset) {
    std::cout << "in offset:" << offset.X << "," << offset.Y << "," << offset.Z << std::endl;
    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        irr::core::vector3df childPosition = (*it)->getPosition();
        if (childPosition.X == offset.X && childPosition.Y == offset.Y && childPosition.Z == offset.Z) {
            if (*it != NULL) {
                std::cout << "node not null from children FOUND" << std::endl;
            }
            return *it;
        }

        offset.X -= childPosition.X;
        offset.Y -= childPosition.Y;
        offset.Z -= childPosition.Z;
        std::cout << "after minus offset:" << offset.X << "," << offset.Y << "," << offset.Z << std::endl;
        irr::scene::ISceneNode* node = getNodeByOffset(*it, offset);
        if (node != NULL) {
            std::cout << "node not null from children" << std::endl;
            return node;
        }
        offset.X += childPosition.X;
        offset.Y += childPosition.Y;
        offset.Z += childPosition.Z;
    }

    std::cout << "node null from function" << std::endl;
    return NULL;
}

irr::scene::ISceneNode* CFigure::getNodeByOffset(irr::core::vector3di offset) {
    if (offset == irr::core::vector3di()) {
        return figureParentNode;
    }

    return getNodeByOffset(figureParentNode, offset);
}

void CFigure::removeNodeByOffset(irr::core::vector3di& offset) {
    irr::core::vector3di rememberedParentPosition;
    if (offset == irr::core::vector3di()) {
        rememberedParentPosition = parentNodeFieldPosition;
    }

    removeNodeFromFigure(getNodeByOffset(offset));
    if (offset == irr::core::vector3di()) {
        offset = rememberedParentPosition;
        return;
    }

    std::cout << "after remove offset:" << std::endl;
    std::cout << offset.X << ", " << offset.Y << ", " << offset.Z << std::endl;

    // set field position to empty    
    /*offset.X /= getScale().X;
    offset.X /= getScale().X;
    offset.X /= getScale().X;*/
    irr::core::vector3di tempVector;
    irr::core::vector3di outputPosition;
    if (getRotation() == irr::core::vector3df()) {
        outputPosition.X = offset.X / getScale().X;
        outputPosition.Y = offset.Y / getScale().Y;
        outputPosition.Z = offset.Z / getScale().Z;
    } else {
        // calculate child rotation offset
        if (getRotation().X != 0) {
            tempVector.X = 0;
            tempVector.Y = offset.Y / getScale().Y;
            tempVector.Z = 0;
            tempVector.rotateYZBy(getRotation().X, irr::core::vector3di(0, 0, 0));
            outputPosition += tempVector;

            tempVector.X = 0;
            tempVector.Y = 0;
            tempVector.Z = offset.Z / getScale().Z;
            tempVector.rotateYZBy(getRotation().X, irr::core::vector3di(0, 0, 0));
            outputPosition += tempVector;
        }

        if (getRotation().Y != 0) {
            tempVector.X = offset.X / getScale().X;
            tempVector.Y = 0;
            tempVector.Z = 0;
            tempVector.rotateXZBy(getRotation().Y, irr::core::vector3di(0, 0, 0));
            outputPosition += tempVector;

            tempVector.X = 0;
            tempVector.Y = 0;
            tempVector.Z = offset.Z / getScale().Z;
            tempVector.rotateXZBy(getRotation().Y, irr::core::vector3di(0, 0, 0));
            outputPosition += tempVector;
        }

        if (getRotation().Z != 0) {
            tempVector.X = offset.X / getScale().X;
            tempVector.Y = 0;
            tempVector.Z = 0;
            tempVector.rotateXYBy(getRotation().Z, irr::core::vector3di(0, 0, 0));
            outputPosition += tempVector;

            tempVector.X = 0;
            tempVector.Y = offset.Y / getScale().Y;
            tempVector.Z = 0;
            tempVector.rotateXYBy(getRotation().Z, irr::core::vector3di(0, 0, 0));
            outputPosition += tempVector;
        }
    }
    outputPosition += parentNodeFieldPosition;
    offset = outputPosition;
    std::cout << "field position to empty:" << std::endl;
    std::cout << offset.X << ", " << offset.Y << ", " << offset.Z << std::endl;
    // must return
}

void clip(irr::scene::ISceneNode* node, irr::core::vector3di vector) {
    irr::core::vector3df position = node->getPosition();
    position.X -= vector.X;
    position.Y -= vector.Y;
    position.Z -= vector.Z;
    node->setPosition(position);

    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    // affect on all upper children with gravity
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        clip(*it, vector);
    }
}

bool CFigure::g() {
    if (gparent != NULL) {
        std::cout << "before g" << std::endl;
        std::cout << "parent position" << std::endl;
        std::cout << parentNodeFieldPosition.X << "," << parentNodeFieldPosition.Y << "," << parentNodeFieldPosition.Z << std::endl;
        std::cout << "gparent rel. position" << std::endl;
        std::cout << gparent->getPosition().X << "," << gparent->getPosition().Y << "," << gparent->getPosition().Z << std::endl;
        std::cout << "goffset" << std::endl;
        std::cout << goffset.X << "," << goffset.Y << "," << goffset.Z << std::endl;
        for (irr::u32 i = 0; i < fieldPositions.size(); ++i) {
            std::cout << fieldPositions[i].X << "," << fieldPositions[i].Y << "," << fieldPositions[i].Z << std::endl;
        }

        if (gparent == figureParentNode) {
            irr::core::list<irr::scene::ISceneNode*> children = gparent->getChildren();
            // affect on all lower children with antigravity
            for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
                irr::core::vector3df relPosition = (*it)->getPosition();
                if (relPosition.X != 0) {
                    relPosition.X = figureSize.X;
                } else if (relPosition.Y != 0) {
                    relPosition.Y = figureSize.Y;
                } else if (relPosition.Z != 0) {
                    relPosition.Z = figureSize.Z;
                }
                clip(*it, irr::core::vector3di(relPosition.X, relPosition.Y, relPosition.Z));
            }

            constructNodesFieldPositions(parentNodeFieldPosition);            
        } else {
            irr::core::vector3df relPosition = gparent->getPosition();
            if (relPosition.X != 0) {
                relPosition.X = figureSize.X;
            } else if (relPosition.Y != 0) {
                relPosition.Y = figureSize.Y;
            } else if (relPosition.Z != 0) {
                relPosition.Z = figureSize.Z;
            }
            
            clip(gparent, irr::core::vector3di(relPosition.X, relPosition.Y, relPosition.Z));
            constructNodesFieldPositions(parentNodeFieldPosition);
        }


        std::cout << "after g" << std::endl;
        for (irr::u32 i = 0; i < fieldPositions.size(); ++i) {
            std::cout << fieldPositions[i].X << "," << fieldPositions[i].Y << "," << fieldPositions[i].Z << std::endl;
        }       
        gparent = NULL;
        return true;
    }
    return false;
}

void CFigure::removeNodeFromFigure(irr::scene::ISceneNode* nodeToDelete, bool withIntegrityCheck) {
    if (figureParentNode == NULL) {
        return;
    }

    // if node has no children - simply remove
    if (nodeToDelete->getChildren().size() == 0) {
        nodeToDelete->remove();
        if (nodeToDelete == figureParentNode) {
            figureParentNode = NULL;
            fieldPositions.clear();
            return;
        }
        recalculateBoundingBox();
        constructNodesFieldPositions(parentNodeFieldPosition);
        return;
    }

    // if withIntegrityCheck == "true", you can't delete nodes that has children
    if (withIntegrityCheck) {
        return;
    }

    // if node has children - all of them become children of node-to-delete-parent
    // also recalculate their relative positions
    // special case if we need to remove figure root node
    irr::core::list<irr::scene::ISceneNode*> children = nodeToDelete->getChildren();
    // affect on all upper children with gravity
    bool nodesBelowExists = false;
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        irr::core::vector3df res;
        if (nodeToDelete->getRotation() == irr::core::vector3df()) {
            res.X = (*it)->getPosition().X;
            res.Y = (*it)->getPosition().Y;
            res.Z = (*it)->getPosition().Z;
        } else {
            irr::core::vector3di tempVector;
            // calculate child rotation offset
            if (nodeToDelete->getRotation().X != 0) {
                tempVector.X = 0;
                tempVector.Y = (*it)->getPosition().Y;
                tempVector.Z = 0;
                tempVector.rotateYZBy(nodeToDelete->getRotation().X, irr::core::vector3di(0, 0, 0));
                res.X += tempVector.X;
                res.Y += tempVector.Y;
                res.Z += tempVector.Z;

                tempVector.X = 0;
                tempVector.Y = 0;
                tempVector.Z = (*it)->getPosition().Z;
                tempVector.rotateYZBy(nodeToDelete->getRotation().X, irr::core::vector3di(0, 0, 0));
                res.X += tempVector.X;
                res.Y += tempVector.Y;
                res.Z += tempVector.Z;
            }

            if (nodeToDelete->getRotation().Y != 0) {
                tempVector.X = (*it)->getPosition().X;
                tempVector.Y = 0;
                tempVector.Z = 0;
                tempVector.rotateXZBy(nodeToDelete->getRotation().Y, irr::core::vector3di(0, 0, 0));
                res.X += tempVector.X;
                res.Y += tempVector.Y;
                res.Z += tempVector.Z;

                tempVector.X = 0;
                tempVector.Y = 0;
                tempVector.Z = (*it)->getPosition().Z;
                tempVector.rotateXZBy(nodeToDelete->getRotation().Y, irr::core::vector3di(0, 0, 0));
                res.X += tempVector.X;
                res.Y += tempVector.Y;
                res.Z += tempVector.Z;
            }

            if (nodeToDelete->getRotation().Z != 0) {
                tempVector.X = (*it)->getPosition().X;
                tempVector.Y = 0;
                tempVector.Z = 0;
                tempVector.rotateXYBy(nodeToDelete->getRotation().Z, irr::core::vector3di(0, 0, 0));
                res.X += tempVector.X;
                res.Y += tempVector.Y;
                res.Z += tempVector.Z;

                tempVector.X = 0;
                tempVector.Y = (*it)->getPosition().Y;
                tempVector.Z = 0;
                tempVector.rotateXYBy(nodeToDelete->getRotation().Z, irr::core::vector3di(0, 0, 0));
                res.X += tempVector.X;
                res.Y += tempVector.Y;
                res.Z += tempVector.Z;
            }
        }
        std::cout << "res: " << res.X << ", " << res.Y << ", " << res.Z << std::endl;
        if (res.Y < 0) {
            nodesBelowExists = true;
            break;
        }
    }
    if (nodesBelowExists) {
        for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
            irr::core::vector3df res;
            if (nodeToDelete->getRotation() == irr::core::vector3df()) {
                res.X = (*it)->getPosition().X;
                res.Y = (*it)->getPosition().Y;
                res.Z = (*it)->getPosition().Z;
            } else {
                irr::core::vector3di tempVector;
                // calculate child rotation offset
                if (nodeToDelete->getRotation().X != 0) {
                    tempVector.X = 0;
                    tempVector.Y = (*it)->getPosition().Y;
                    tempVector.Z = 0;
                    tempVector.rotateYZBy(nodeToDelete->getRotation().X, irr::core::vector3di(0, 0, 0));
                    res.X += tempVector.X;
                    res.Y += tempVector.Y;
                    res.Z += tempVector.Z;

                    tempVector.X = 0;
                    tempVector.Y = 0;
                    tempVector.Z = (*it)->getPosition().Z;
                    tempVector.rotateYZBy(nodeToDelete->getRotation().X, irr::core::vector3di(0, 0, 0));
                    res.X += tempVector.X;
                    res.Y += tempVector.Y;
                    res.Z += tempVector.Z;
                }

                if (nodeToDelete->getRotation().Y != 0) {
                    tempVector.X = (*it)->getPosition().X;
                    tempVector.Y = 0;
                    tempVector.Z = 0;
                    tempVector.rotateXZBy(nodeToDelete->getRotation().Y, irr::core::vector3di(0, 0, 0));
                    res.X += tempVector.X;
                    res.Y += tempVector.Y;
                    res.Z += tempVector.Z;

                    tempVector.X = 0;
                    tempVector.Y = 0;
                    tempVector.Z = (*it)->getPosition().Z;
                    tempVector.rotateXZBy(nodeToDelete->getRotation().Y, irr::core::vector3di(0, 0, 0));
                    res.X += tempVector.X;
                    res.Y += tempVector.Y;
                    res.Z += tempVector.Z;
                }

                if (nodeToDelete->getRotation().Z != 0) {
                    tempVector.X = (*it)->getPosition().X;
                    tempVector.Y = 0;
                    tempVector.Z = 0;
                    tempVector.rotateXYBy(nodeToDelete->getRotation().Z, irr::core::vector3di(0, 0, 0));
                    res.X += tempVector.X;
                    res.Y += tempVector.Y;
                    res.Z += tempVector.Z;

                    tempVector.X = 0;
                    tempVector.Y = (*it)->getPosition().Y;
                    tempVector.Z = 0;
                    tempVector.rotateXYBy(nodeToDelete->getRotation().Z, irr::core::vector3di(0, 0, 0));
                    res.X += tempVector.X;
                    res.Y += tempVector.Y;
                    res.Z += tempVector.Z;
                }
            }
            std::cout << "res: " << res.X << ", " << res.Y << ", " << res.Z << std::endl;
            if (res.Y > 0) {
                irr::core::vector3df res2;
                if (nodeToDelete->getRotation() == irr::core::vector3df()) {
                    res.X = (*it)->getPosition().X;
                    res.Y = (*it)->getPosition().Y;
                    res.Z = (*it)->getPosition().Z;
                } else {
                    irr::core::vector3di tempVector;
                    // calculate child rotation offset
                    if (nodeToDelete->getRotation().X != 0) {
                        tempVector.X = 0;
                        tempVector.Y = res.Y;
                        tempVector.Z = 0;
                        tempVector.rotateYZBy(-nodeToDelete->getRotation().X, irr::core::vector3di(0, 0, 0));
                        res2.X += tempVector.X;
                        res2.Y += tempVector.Y;
                        res2.Z += tempVector.Z;

                        tempVector.X = 0;
                        tempVector.Y = 0;
                        tempVector.Z = res.Z;
                        tempVector.rotateYZBy(-nodeToDelete->getRotation().X, irr::core::vector3di(0, 0, 0));
                        res2.X += tempVector.X;
                        res2.Y += tempVector.Y;
                        res2.Z += tempVector.Z;
                    }

                    if (nodeToDelete->getRotation().Y != 0) {
                        tempVector.X = res.X;
                        tempVector.Y = 0;
                        tempVector.Z = 0;
                        tempVector.rotateXZBy(-nodeToDelete->getRotation().Y, irr::core::vector3di(0, 0, 0));
                        res2.X += tempVector.X;
                        res2.Y += tempVector.Y;
                        res2.Z += tempVector.Z;

                        tempVector.X = 0;
                        tempVector.Y = 0;
                        tempVector.Z = res.Z;
                        tempVector.rotateXZBy(-nodeToDelete->getRotation().Y, irr::core::vector3di(0, 0, 0));
                        res2.X += tempVector.X;
                        res2.Y += tempVector.Y;
                        res2.Z += tempVector.Z;
                    }

                    if (nodeToDelete->getRotation().Z != 0) {
                        tempVector.X = res.X;
                        tempVector.Y = 0;
                        tempVector.Z = 0;
                        tempVector.rotateXYBy(-nodeToDelete->getRotation().Z, irr::core::vector3di(0, 0, 0));
                        res2.X += tempVector.X;
                        res2.Y += tempVector.Y;
                        res2.Z += tempVector.Z;

                        tempVector.X = 0;
                        tempVector.Y = res.Y;
                        tempVector.Z = 0;
                        tempVector.rotateXYBy(-nodeToDelete->getRotation().Z, irr::core::vector3di(0, 0, 0));
                        res2.X += tempVector.X;
                        res2.Y += tempVector.Y;
                        res2.Z += tempVector.Z;
                    }
                }
                irr::core::vector3di tempVector;
                tempVector.X = res2.X;
                tempVector.Y = res2.Y;
                tempVector.Z = res2.Z;
                goffset = tempVector;
                gparent = *it;
                //tempVector.rotateXYBy(-nodeToDelete->getRotation().Z, irr::core::vector3di(0, 0, 0));
                //std::cout << "res2: " << tempVector.X << ", " << tempVector.Y << ", " << tempVector.Z << std::endl;
                //clip(*it, tempVector);
            }
        }
    }

    if (nodeToDelete == figureParentNode) {
        figureParentNode = (*children.begin());
        for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin() + 1; it != children.end(); ++it) {
            irr::core::vector3df newposition = (*it)->getPosition() - figureParentNode->getPosition();
            std::cout << newposition.X << ", " << newposition.Y << ", " << newposition.Z << std::endl;
            (*it)->setParent(figureParentNode);
            (*it)->setPosition(newposition);
        }

        // extremely important that newParentPosition has vector3dI type, otherwise after
        // rotateXXBy() method call you'll have wrong values
        irr::core::vector3di newParentPosition;
        //std::cout << "parent rel. pos. (before rot)" << std::endl;
        //std::cout << newParentPosition.X << ", " << newParentPosition.Y << ", " << newParentPosition.Z << std::endl;
        std::cout << " . . ( rot)" << std::endl;
        std::cout << nodeToDelete->getRotation().X << ", " << nodeToDelete->getRotation().Y << ", " << nodeToDelete->getRotation().Z << std::endl;

        if (nodeToDelete->getRotation() == irr::core::vector3df()) {
            newParentPosition.X = (figureParentNode->getPosition() / nodeToDelete->getScale()).X;
            newParentPosition.Y = (figureParentNode->getPosition() / nodeToDelete->getScale()).Y;
            newParentPosition.Z = (figureParentNode->getPosition() / nodeToDelete->getScale()).Z;
        } else {
            irr::core::vector3di tempVector;
            // calculate child rotation offset
            if (nodeToDelete->getRotation().X != 0) {
                tempVector.X = 0;
                tempVector.Y = (figureParentNode->getPosition() / nodeToDelete->getScale()).Y;
                tempVector.Z = 0;
                tempVector.rotateYZBy(nodeToDelete->getRotation().X, irr::core::vector3di(0, 0, 0));
                newParentPosition += tempVector;

                tempVector.X = 0;
                tempVector.Y = 0;
                tempVector.Z = (figureParentNode->getPosition() / nodeToDelete->getScale()).Z;
                tempVector.rotateYZBy(nodeToDelete->getRotation().X, irr::core::vector3di(0, 0, 0));
                newParentPosition += tempVector;
            }

            if (nodeToDelete->getRotation().Y != 0) {
                tempVector.X = (figureParentNode->getPosition() / nodeToDelete->getScale()).X;
                tempVector.Y = 0;
                tempVector.Z = 0;
                tempVector.rotateXZBy(nodeToDelete->getRotation().Y, irr::core::vector3di(0, 0, 0));
                newParentPosition += tempVector;

                tempVector.X = 0;
                tempVector.Y = 0;
                tempVector.Z = (figureParentNode->getPosition() / nodeToDelete->getScale()).Z;
                tempVector.rotateXZBy(nodeToDelete->getRotation().Y, irr::core::vector3di(0, 0, 0));
                newParentPosition += tempVector;
            }

            if (nodeToDelete->getRotation().Z != 0) {
                tempVector.X = (figureParentNode->getPosition() / nodeToDelete->getScale()).X;
                tempVector.Y = 0;
                tempVector.Z = 0;
                tempVector.rotateXYBy(nodeToDelete->getRotation().Z, irr::core::vector3di(0, 0, 0));
                newParentPosition += tempVector;

                tempVector.X = 0;
                tempVector.Y = (figureParentNode->getPosition() / nodeToDelete->getScale()).Y;
                tempVector.Z = 0;
                tempVector.rotateXYBy(nodeToDelete->getRotation().Z, irr::core::vector3di(0, 0, 0));
                newParentPosition += tempVector;
            }
        }

        std::cout << "parent rel. pos. (after rot)" << std::endl;
        std::cout << newParentPosition.X << ", " << newParentPosition.Y << ", " << newParentPosition.Z << std::endl;
        parentNodeFieldPosition += newParentPosition;
        newParentPosition.X *= nodeToDelete->getScale().X * nodeToDelete->getScale().X;
        newParentPosition.Y *= nodeToDelete->getScale().Y * nodeToDelete->getScale().Y;
        newParentPosition.Z *= nodeToDelete->getScale().Z * nodeToDelete->getScale().Z;
        newParentPosition.X += nodeToDelete->getPosition().X;
        newParentPosition.Y += nodeToDelete->getPosition().Y;
        newParentPosition.Z += nodeToDelete->getPosition().Z;
        irr::core::vector3df newPosition;
        newPosition.X = newParentPosition.X;
        newPosition.Y = newParentPosition.Y;
        newPosition.Z = newParentPosition.Z;
        boxOffset = -(figureParentNode->getPosition() * nodeToDelete->getScale());
        figureParentNode->setParent(nodeToDelete->getParent());
        figureParentNode->setPosition(newPosition);
        figureParentNode->setRotation(nodeToDelete->getRotation());
        figureParentNode->setScale(nodeToDelete->getScale());
        irr::scene::ISceneNodeAnimatorList animators = nodeToDelete->getAnimators();
        for (irr::scene::ISceneNodeAnimatorList::Iterator it = animators.begin(); it != animators.end(); ++it) {
            figureParentNode->addAnimator(*it);
        }
    } else {
        for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
            irr::core::vector3df newposition = (*it)->getPosition() + nodeToDelete->getPosition();
            (*it)->setParent(nodeToDelete->getParent());
            (*it)->setPosition(newposition);
        }
    }

    constructNodesFieldPositions(parentNodeFieldPosition);
    nodeToDelete->remove();
}

void CFigure::setCubeColour(irr::scene::ISceneNode* node, irr::video::SColor colour, irr::u32 borderSize) {
    irr::video::ITexture* texture = node->getMaterial(0).getTexture(0);
    irr::video::SColor* nodeTexturePixels = (irr::video::SColor*)texture->lock(irr::video::ETLM_WRITE_ONLY);
    for (irr::u32 i = borderSize; i < texture->getSize().Width - borderSize; ++i) {
        for (irr::u32 j = borderSize; j < texture->getSize().Height - borderSize; ++j) {
            nodeTexturePixels[i * texture->getSize().Width + j].set(colour.color);
        }
    }
    texture->unlock();
}

