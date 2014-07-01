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
        irr::io::IFileSystem* fileSystem, irr::f32 figureSize, irr::io::path figureFile) :
irr::scene::ISceneNode(parent, sceneManager, id), figureName(L""), comment(L""), fileSystem(fileSystem), figureParentNode(NULL) {
#ifdef _DEBUG
    setDebugName("CFigure");
#endif

    sceneNodeFactory = sceneManager->getSceneNodeFactory(0);
    videoDriver = sceneManager->getVideoDriver();
    this->figureSize = irr::core::vector3df(figureSize, figureSize, figureSize);

    if (figureFile == "" || !createFigureFromFile(figureFile)) {
        // hope we get default sceneManager with default sceneNodeFactory which has method to create default cube sceneNode
        figureParentNode = sceneNodeFactory->addSceneNode(irr::scene::ESNT_CUBE, getParent());
        figureParentNode->setScale(this->figureSize);
        Box = figureParentNode->getBoundingBox();
        nodeType = irr::scene::ESNT_CUBE;
    }

    boxOffset.set(0.0f, 0.0f, 0.0f);
}

CFigure::~CFigure() {
}

// TODO closing control

bool CFigure::createFigureFromFile(irr::io::path pathToFile) {
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

    nodeType = irr::scene::ESNT_UNKNOWN;

    irr::scene::ISceneNode* currentParentNode = NULL;
    //while there is more to read
    while (xmlReader->read()) {
        //check the node type
        switch (xmlReader->getNodeType()) {
            case irr::io::EXN_ELEMENT:
            {
                if (currentSection.empty() && figureTag.equals_ignore_case(xmlReader->getNodeName())) {
                    currentSection = figureTag;
                    irr::core::stringw typeAttributeValue = xmlReader->getAttributeValueSafe(L"type");
                    // TODO replace this if-else statements with correct element creation via factory                    
                    if (!typeAttributeValue.empty()) {
                        if (typeAttributeValue.equals_ignore_case(irr::core::stringw(L"cube"))) {
                            nodeType = irr::scene::ESNT_CUBE;
                        } else if (typeAttributeValue.equals_ignore_case(irr::core::stringw(L"sphere"))) {
                            nodeType = irr::scene::ESNT_SPHERE;
                        }
                    } else {
                        xmlReader->drop();
                        return false;
                    }

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

    irr::core::stringw stringNodeType;
    if (nodeType == irr::scene::ESNT_CUBE) {
        stringNodeType = L"cube";
    } else if (irr::scene::ESNT_SPHERE) {
        stringNodeType = L"sphere";
    } else {
        return false;
    }

    xwriter->writeElement(L"figure", false, L"type", stringNodeType.c_str(), L"name", figureName.c_str(), L"comment", comment.c_str());
    xwriter->writeLineBreak();

    saveNodeChildrenToFile(xwriter, figureParentNode);

    xwriter->writeClosingTag(L"figure");

    return true;
}

void CFigure::recalculateBoundingBox() {
    calculateChildrenBoundingBox(figureParentNode, Box);
    boxOffset.set(0.0f, 0.0f, 0.0f);
    /*std::cout << "__" << std::endl;
    std::cout << Box.MinEdge.X << std::endl;
    std::cout << Box.MinEdge.Y << std::endl;
    std::cout << Box.MinEdge.Z << std::endl;

    std::cout << Box.MaxEdge.X << std::endl;
    std::cout << Box.MaxEdge.Y << std::endl;
    std::cout << Box.MaxEdge.Z << std::endl;*/

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

        /*aabb.MaxEdge = childAABB.MaxEdge;//maxEdge;
    aabb.MinEdge = childAABB.MinEdge;//minEdge;
    return;*/

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
        /*std::cout << "__" << std::endl;
        std::cout << minEdge.X << std::endl;
        std::cout << minEdge.Y << std::endl;
        std::cout << minEdge.Z << std::endl;

        std::cout << maxEdge.X << std::endl;
        std::cout << maxEdge.Y << std::endl;
        std::cout << maxEdge.Z << std::endl;*/


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
}

bool CFigure::outOfFieldBorders(CGameFieldManager* gameFieldManager, irr::core::vector3di position) {
    irr::core::vector3di fieldSize = gameFieldManager->getFieldSize();
    return ((position.X < 0 || position.Y < 0 || position.Z < 0) || ((position.X > fieldSize.X) || (position.Y > fieldSize.Y) || (position.Z > fieldSize.Z)));
}

void CFigure::construct(irr::scene::ISceneNode* node, irr::core::vector3di nodeFieldPosition) {
    fieldPositions.push_back(nodeFieldPosition);

    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        irr::core::vector3di childFieldPosition;
        childFieldPosition.X = ((*it)->getPosition() / figureSize).X;
        childFieldPosition.Y = ((*it)->getPosition() / figureSize).Y;
        childFieldPosition.Z = ((*it)->getPosition() / figureSize).Z;
        childFieldPosition += nodeFieldPosition;

        construct(*it, childFieldPosition);
    }
}

void CFigure::constructNodesFieldPositions(irr::core::vector3di parentNodeFieldPosition) {
    this->parentNodeFieldPosition = parentNodeFieldPosition;
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
                /*extremePositions.push_back(fieldPositions[i]);
                newPositionFound = true;
                break; */
                continue;
            }
            
            /*std::cout << "ffffffffffff " << i << std::endl;
            std::cout << fieldPositions[i].X << std::endl;
            std::cout << fieldPositions[i].Y << std::endl;
            std::cout << fieldPositions[i].Z << std::endl;
            
            std::cout << "eeeeeeeeeeeeeee " << j << std::endl;
            std::cout << extremePositions[j].X << std::endl;
            std::cout << extremePositions[j].Y << std::endl;
            std::cout << extremePositions[j].Z << std::endl;*/
            
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
            //newPositionFound = true;
        }
        
        // found nothing in extreme positions array
        if (!newPositionFound) {
            extremePositions.push_back(fieldPositions[i]);
        }
    } 
}

// simplier way

/*
 construct list<vector3di> fieldPositions of _every_ node, every time figure reconstructs
 * figure change root node position on field
 * figure loses its nodes
 
 run through it if you want to get extreme positions 
 */

bool CFigure::isPossibleToMoveInDirection(CGameFieldManager* gameFieldManager, irr::core::vector3di direction, irr::scene::ISceneNode* node, irr::core::vector3di nodeFieldPosition, irr::core::list<irr::core::vector3di>& nextFieldPositions) {
    irr::core::vector3di nextNodeFieldPosition = nodeFieldPosition + direction;
    if (outOfFieldBorders(gameFieldManager, nextNodeFieldPosition)) {
        return false;
    }

    // check if already exists more up/low/right/left/etc.-direction field position
    /*
     example:
     we try to move this figure in right direction. "x" is root node
     0-0
       |
       x-0-*
     
     so we have to check if current node (marked as *) has more righter direction than others (on same line, cause we more in right direction)
     if we have to check nodes in down direction, we should check possible current node competitors in vertical line with current node    
     */
    bool positionFound = false;
    for (irr::core::list<irr::core::vector3di>::Iterator it = nextFieldPositions.begin(); it != nextFieldPositions.end(); ++it) {
        // sift unnecessary nodes
        if (// Movement in X axis. Check if other nodes don't have same Y and Z coordinates
                (direction.X != 0 && ((*it).Y != nextNodeFieldPosition.Y || (*it).Z != nextNodeFieldPosition.Z)) ||
                // Movement in Y axis. Check if other nodes don't have same X and Z coordinates
                (direction.Y != 0 && ((*it).X != nextNodeFieldPosition.X || (*it).Z != nextNodeFieldPosition.Z)) ||
                // Movement in Z axis. Check if other nodes don't have same X and Y coordinates
                (direction.Z != 0 && ((*it).X != nextNodeFieldPosition.X || (*it).Y != nextNodeFieldPosition.Y))) {
            continue;
        }

        // cause we try to find most right/up/down/etc. nodes, one if-statement is all we need
        if (// movement in left direction, seek for more left position
                (direction.X < 0 && nextNodeFieldPosition.X < (*it).X) ||
                // movement in right direction, seek for more right position
                (direction.X > 0 && nextNodeFieldPosition.X > (*it).X) ||
                // movement in down direction, seek ----//-----
                (direction.Y < 0 && nextNodeFieldPosition.Y < (*it).Y) ||
                // movement in up direction
                (direction.Y > 0 && nextNodeFieldPosition.Y > (*it).Y) ||
                // movement in back direction
                (direction.Z < 0 && nextNodeFieldPosition.Z < (*it).Z) ||
                // movement in front direction
                (direction.Z > 0 && nextNodeFieldPosition.Z > (*it).Z)) {
            // if all is fine and we attempt to replace extreme position, check is field in this position empty
            if (gameFieldManager->getFieldValue(nextNodeFieldPosition) != EGF_EMPTY) {
                return false;
            }
            nextFieldPositions.insert_before(it, nextNodeFieldPosition);
            nextFieldPositions.erase(it);
            break;
        }
        positionFound = true;
    }

    // in case of empty list or no other in-one-line nodes
    if (!positionFound) {
        // check if this position filled with something
        if (gameFieldManager->getFieldValue(nextNodeFieldPosition) != EGF_EMPTY) {
            return false;
        }
        nextFieldPositions.push_back(nextNodeFieldPosition);
    }

    irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
    for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin(); it != children.end(); ++it) {
        irr::core::vector3di childFieldPosition;
        childFieldPosition.X = ((*it)->getPosition() / figureSize).X;
        childFieldPosition.Y = ((*it)->getPosition() / figureSize).Y;
        childFieldPosition.Z = ((*it)->getPosition() / figureSize).Z;
        childFieldPosition += nodeFieldPosition;
        /*if (outOfFieldBorders(gameFieldManager, childFieldPosition + direction)) {
            return false;
        }

        if (gameFieldManager->getFieldValue(nodeFieldPosition + direction) != EGF_FIGURE_TYPE::EGF_EMPTY) {
            return false;
        }*/

        if (!isPossibleToMoveInDirection(gameFieldManager, direction, *it, childFieldPosition, nextFieldPositions)) {
            return false;
        }
    }

    return true;
}

bool CFigure::isPossibleToMoveInDirection(CGameFieldManager* gameFieldManager, irr::core::vector3di direction, irr::core::list<irr::core::vector3di>& nextFieldPositions) {
    return isPossibleToMoveInDirection(gameFieldManager, direction, figureParentNode, parentNodeFieldPosition, nextFieldPositions);
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

const irr::core::aabbox3d<irr::f32>& CFigure::getBoundingBox() const {
    return Box;
}

const irr::core::aabbox3d<irr::f32> CFigure::getTransformedBoundingBox() const {
    irr::core::aabbox3df box = Box;
    /*std::cout << "positi parent" << std::endl;
    std::cout << figureParentNode->getPosition().X << std::endl;
    std::cout << figureParentNode->getPosition().Y << std::endl;
    std::cout << figureParentNode->getPosition().Z << std::endl;*/

    irr::core::matrix4 transformationMatrix = figureParentNode->getRelativeTransformation();
    /*std::cout << transformationMatrix.getTranslation().X << std::endl;
    std::cout << transformationMatrix.getTranslation().Y << std::endl;
    std::cout << transformationMatrix.getTranslation().Z << std::endl;
    transformationMatrix.setTranslation(transformationMatrix.getTranslation() + boxOffset);
    std::cout << transformationMatrix.getTranslation().X << std::endl;
    std::cout << transformationMatrix.getTranslation().Y << std::endl;
    std::cout << transformationMatrix.getTranslation().Z << std::endl;*/
    transformationMatrix.transformBoxEx(box);
    box.MinEdge += boxOffset;
    box.MaxEdge += boxOffset;
    /*    std::cout << "positi box" << std::endl;
    std::cout << box.MinEdge.X << std::endl;
    std::cout << box.MinEdge.Y << std::endl;
    std::cout << box.MinEdge.Z << std::endl;
    
    std::cout << box.MaxEdge.X << std::endl;
    std::cout << box.MaxEdge.Y << std::endl;
    std::cout << box.MaxEdge.Z << std::endl;*/
    return box;
}

irr::u32 CFigure::getMaterialCount() const {
    return 1;
}

irr::video::SMaterial& CFigure::getMaterial(irr::u32 num) {
    return Material;
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
    figureParentNode->setScale(scale);
}

const irr::core::vector3df& CFigure::getScale() const {
    return figureParentNode->getScale();
}

const irr::core::vector3df& CFigure::getRotation() const {
    return figureParentNode->getRotation();
}

void CFigure::setRotation(const irr::core::vector3df& rotation) {
    figureParentNode->setRotation(rotation);
}

void CFigure::removeNodeFromFigure(irr::scene::ISceneNode* nodeToDelete, bool withIntegrityCheck) {
    if (figureParentNode == NULL) {
        return;
    }

    // if node has no children - simply remove
    if (nodeToDelete->getChildren().size() == 0) {
        nodeToDelete->remove();
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
    if (nodeToDelete == figureParentNode) {
        figureParentNode = (*children.begin());
        for (irr::core::list<irr::scene::ISceneNode*>::Iterator it = children.begin() + 1; it != children.end(); ++it) {
            irr::core::vector3df newposition = (*it)->getPosition() - figureParentNode->getPosition();
            (*it)->setParent(figureParentNode);
            (*it)->setPosition(newposition);
        }

        parentNodeFieldPosition.X += (figureParentNode->getPosition() / nodeToDelete->getScale()).X;
        parentNodeFieldPosition.Y += (figureParentNode->getPosition() / nodeToDelete->getScale()).Y;
        parentNodeFieldPosition.Z += (figureParentNode->getPosition() / nodeToDelete->getScale()).Z;
        boxOffset = -(figureParentNode->getPosition() * nodeToDelete->getScale());
        figureParentNode->setParent(nodeToDelete->getParent());
        figureParentNode->setPosition(nodeToDelete->getPosition() + (figureParentNode->getPosition() * nodeToDelete->getScale()));
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

