#include "headers/mainwindow.h"
#include "headers/CGridSceneNode.h"
#include "headers/CFigure.h"

#include <QtGui/QApplication>
#include <iostream>
using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

class MyEventReceiver : public IEventReceiver {
public:

    MyEventReceiver(scene::ISceneNode* terrain, scene::ISceneNode* skybox, scene::ISceneNode* skydome, ISceneNode* cube) :
    Terrain(terrain), Skybox(skybox), Skydome(skydome), cube(cube), showBox(true), showDebug(false) {
        Skybox->setVisible(showBox);
        Skydome->setVisible(!showBox);
    }

    bool OnEvent(const SEvent& event) {
        // check if user presses the key 'W' or 'D'

        if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown) {
            core::vector3df terrainPosition = Terrain->getPosition();
            core::vector3df previousPosition = cube->getPosition();
            switch (event.KeyInput.Key) {
                case irr::KEY_KEY_W: // switch wire frame mode
                    Terrain->setMaterialFlag(video::EMF_WIREFRAME,
                            !Terrain->getMaterial(0).Wireframe);
                    Terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
                    return true;
                case irr::KEY_KEY_P: // switch wire frame mode
                    Terrain->setMaterialFlag(video::EMF_POINTCLOUD,
                            !Terrain->getMaterial(0).PointCloud);
                    Terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
                    return true;
                case irr::KEY_KEY_D: // toggle detail map
                    Terrain->setMaterialType(
                            Terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
                            video::EMT_DETAIL_MAP : video::EMT_SOLID);
                    return true;
                case irr::KEY_KEY_S: // toggle skies
                    showBox = !showBox;
                    Skybox->setVisible(showBox);
                    Skydome->setVisible(!showBox);
                    return true;
                case irr::KEY_KEY_X: // toggle debug information
                    showDebug = !showDebug;
                    Terrain->setDebugDataVisible(showDebug ? scene::EDS_BBOX_ALL : scene::EDS_OFF);
                    return true;
                case irr::KEY_SPACE:
                    cube->setPosition(core::vector3df(previousPosition.X, terrainPosition.Y + 1000, previousPosition.Z));
                    return true;
                case irr::KEY_PLUS:
                    increaseAnimatorGravity();
                    return true;
                case irr::KEY_MINUS:
                    decreaseAnimatorGravity();
                    return true;
                default:
                    break;
            }
        }

        return false;
    }

private:

    void increaseAnimatorGravity(/*ISceneNode* node,*/ f32 value = 0.1) {
        changeAnimatorGravity(/*node,*/ -value);
    }

    void decreaseAnimatorGravity(/*ISceneNode* node,*/ f32 value = 0.1) {
        changeAnimatorGravity(/*node,*/ value);
    }

    void changeAnimatorGravity(/*ISceneNode* node,*/ f32 value = 0.1) {
        core::list<ISceneNodeAnimator*> cubeAnimators = /*node*/cube->getAnimators();
        for (core::list<ISceneNodeAnimator*>::Iterator it = cubeAnimators.begin(); it != cubeAnimators.end(); ++it) {
            if ((*it)->getType() == ESNAT_COLLISION_RESPONSE) {
                ISceneNodeAnimatorCollisionResponse* animator = static_cast<ISceneNodeAnimatorCollisionResponse*> (*it);
                core::vector3df currentGravity = animator->getGravity();
                // increase gravity affection (increase module-value, not sign)
                // gravity always exists and can't be >= 0, so objects will fly up
                // TODO check zero-gravity value
                if ((currentGravity.Y + value) < ROUNDING_ERROR_f32) {
                    currentGravity.Y += value;
                    animator->setGravity(currentGravity);
                } else {
                    std::cout << "You've achieved minimum gravity value(" << currentGravity.Y << ")" << std::endl;
                }
                break;
            }
        }
    }

    scene::ISceneNode* Terrain;
    scene::ISceneNode* Skybox;
    scene::ISceneNode* Skydome;
    scene::ISceneNode* cube;
    bool showBox;
    bool showDebug;
};

struct SGridBox {
    CGridSceneNode* rightGrid;
    CGridSceneNode* leftGrid;
    CGridSceneNode* downGrid;
    CGridSceneNode* frontGrid;
    CGridSceneNode* backGrid;
    scene::ISceneNode* parent;

    SGridBox(ISceneNode* parent, scene::ISceneManager* smgr, s32 idRight = -1, s32 idLeft = -1, s32 idDown = -1, s32 idFront = -1, s32 idBack = -1) : parent(parent) {
        rightGrid = new CGridSceneNode(parent, smgr, idRight, 1);
        leftGrid = new CGridSceneNode(parent, smgr, idLeft, 1);
        downGrid = new CGridSceneNode(parent, smgr, idDown, 1);
        frontGrid = new CGridSceneNode(parent, smgr, idFront, 1);
        backGrid = new CGridSceneNode(parent, smgr, idBack, 1);

        rightGrid->setRotation(core::vector3df(0.0f, 0.0f, 90.0f));
        leftGrid->setRotation(core::vector3df(0.0f, 0.0f, 90.0f));
        frontGrid->setRotation(core::vector3df(90.0f, 0.0f, 0.0f));
        backGrid->setRotation(core::vector3df(90.0f, 0.0f, 0.0f));

        // set right, left, back & front grid nodes as children for down grid node
        // so changing position will be easier
        downGrid->addChild(rightGrid);
        downGrid->addChild(leftGrid);
        downGrid->addChild(frontGrid);
        downGrid->addChild(backGrid);
    }

    void setPosition(const core::vector3df& newposition) {
        downGrid->setPosition(newposition);
    }

    void setColour(video::SColor newcolour) {
        rightGrid->getGrid(0).setGridColor(newcolour);
        leftGrid->getGrid(0).setGridColor(newcolour);
        downGrid->getGrid(0).setGridColor(newcolour);
        frontGrid->getGrid(0).setGridColor(newcolour);
        backGrid->getGrid(0).setGridColor(newcolour);
    }

    void setGridsSize(const core::dimension2df& newsize) {
        downGrid->setGridsSize(newsize);
        if (newsize.Height < newsize.Width) {
            rightGrid->setGridsSize(core::dimension2df(newsize.Height, newsize.Height));
            rightGrid->setPosition(core::vector3df(newsize.Width / 2, newsize.Height / 2, 0.0f));

            leftGrid->setGridsSize(core::dimension2df(newsize.Height, newsize.Height));
            leftGrid->setPosition(core::vector3df(-newsize.Width / 2, newsize.Height / 2, 0.0f));

            frontGrid->setGridsSize(newsize);
            frontGrid->setPosition(core::vector3df(0.0f, newsize.Height / 2, newsize.Height / 2));

            backGrid->setGridsSize(newsize);
            backGrid->setPosition(core::vector3df(0.0f, newsize.Height / 2, -newsize.Height / 2));
        } else if (newsize.Height > newsize.Width) {
            rightGrid->setGridsSize(newsize);
            rightGrid->setPosition(core::vector3df(newsize.Width / 2, newsize.Width / 2, 0.0f));

            leftGrid->setGridsSize(newsize);
            leftGrid->setPosition(core::vector3df(-newsize.Width / 2, newsize.Width / 2, 0.0f));

            frontGrid->setGridsSize(core::dimension2df(newsize.Width, newsize.Width));
            frontGrid->setPosition(core::vector3df(0.0f, newsize.Width / 2, newsize.Height / 2));

            backGrid->setGridsSize(core::dimension2df(newsize.Width, newsize.Width));
            backGrid->setPosition(core::vector3df(0.0f, newsize.Width / 2, -newsize.Height / 2));
        } else {
            rightGrid->setGridsSize(newsize);
            rightGrid->setPosition(core::vector3df(newsize.Width / 2, newsize.Height / 2, 0.0f));

            leftGrid->setGridsSize(newsize);
            leftGrid->setPosition(core::vector3df(-newsize.Width / 2, newsize.Height / 2, 0.0f));

            frontGrid->setGridsSize(newsize);
            frontGrid->setPosition(core::vector3df(0.0f, newsize.Height / 2, newsize.Width / 2));

            backGrid->setGridsSize(newsize);
            backGrid->setPosition(core::vector3df(0.0f, newsize.Height / 2, -newsize.Width / 2));
        }

        //leftGrid->setRotation(core::vector3df(0, 0, 0));
    }

    void setSpacing(f32 newspacing) {
        rightGrid->getGrid(0).setSpacing(newspacing);
        leftGrid->getGrid(0).setSpacing(newspacing);
        downGrid->getGrid(0).setSpacing(newspacing);
        frontGrid->getGrid(0).setSpacing(newspacing);
        backGrid->getGrid(0).setSpacing(newspacing);
    }

    // so all grids are visible

    void turnOn3DMode() {
        /* downGrid->setVisible(true);
         rightGrid->setVisible(true);
         frontGrid->setVisible(true);
         backGrid->setVisible(true);*/
    }

    // so visible only left grid

    void turnOn2DMode() {
        /*downGrid->setVisible(false);
        rightGrid->setVisible(false);
        frontGrid->setVisible(false);
        backGrid->setVisible(false);*/

        /*downGrid->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);       
        downGrid->setMaterialFlag(video::EMF_LIGHTING, false);
        smgr->getMeshManipulator()->setVertexColorAlpha(downGrid->getGrid(0).g(), 0);*/
    }
};

class CGameField {
public:

    void changeFieldSize(u32 x, u32 y, u32 z) {
        field.reallocate(x * y * z);
        currentSizeX = x;
        currentSizeY = y;
        currentSizeZ = z;
    }

    void deleteNodes(bool twoDimensionalMode) {
        core::array<ISceneNode*> nodesToDelete;
        getNodesToDelete(twoDimensionalMode, nodesToDelete);

        for (u32 i = 0; i < nodesToDelete.size(); ++i) {
            // TODO make figure interface
            /*Figure* figure = dynamic_cast<Fugure*>(nodesToDelete[i]);
            figure->deleteAction();
            figure->remove();*/
            nodesToDelete[i]->remove();
        }
    }

    u8 getFieldValue(u32 x, u32 y, u32 z) const {
        return field[x][y][z];
    }

    void setFieldValue(u8 newValue, u32 x, u32 y, u32 z) {
        field[x][y][z] = newValue;
    }

private:
    //  |y
    //  |    /z
    //  |   /
    //  |  /
    //  | /
    //  |/_ _ _ _ _ x


    // check if line has same values (used for getting list of node to delete)
    // player construct whole line
    // used for 2d-mode

    bool hasLineSameValues(u32 lineNumber) {
        u8 searchValue = field[0][lineNumber][0];
        for (u32 i = 1; i < currentSizeX; ++i) {
            if (searchValue != field[i][lineNumber][0]) {
                return false;
            }
        }

        return true;
    }

    // check if plane has same values (used for getting list of node to delete)
    // player construct whole plane
    // used for 3d-mode

    bool hasPlaneSameValues(u32 planeNumber) {
        u8 searchValue = field[0][planeNumber][0];
        for (u32 i = 1; i < currentSizeX; ++i) {
            for (u32 j = 1; j < currentSizeZ; ++j) {
                if (searchValue != field[i][planeNumber][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    // player has constructed plane (3d-mode) or line (2d-mode) of same elemental blocks
    // so we get list of nodes which must be deleted

    void getNodesToDelete(bool twoDimensionalMode, core::array<scene::ISceneNode*>& outNodes) {
        // get number of lines that already constructed and shich nodes must be removed
        core::array<u32> linesToDeleteArray(currentSizeY);
        for (u32 i = 0; i < currentSizeY; ++i) {
            if (twoDimensionalMode) {
                if (hasLineSameValues(i)) {
                    linesToDeleteArray.push_back(i);
                }
            } else {
                if (hasPlaneSameValues(i)) {
                    linesToDeleteArray.push_back(i);
                }
            }
        }

        // get array of nodes with specific type        
        // TODO here must be correct SceneNode type
        sceneManager->getSceneNodesFromType(ESNT_CUBE, outNodes);

        // check which nodes must be removed
        for (u32 i = 0; i < outNodes.size(); ++i) {
            u32 j = 0;
            for (j; j < linesToDeleteArray.size(); ++j) {
                if (outNodes[i]->getPosition().Y == (fieldYPosition * j)) {
                    break;
                }
            }

            // node must NOT be removed, cause its Y-position not in array of lines to deletion
            if (j == linesToDeleteArray.size()) {
                outNodes.erase(i);
            }
        }
    }

private:
    core::array<core::array<core::array<u32> > > field;

    u32 currentSizeX;
    u32 currentSizeY;
    u32 currentSizeZ;

    // TODO get via terrain->getPosition().Y
    s32 fieldYPosition;

    scene::ISceneManager* sceneManager;

    // TODO get via aabb
    u32 elementalBlockSize;
    core::stringc elementalBlockName;
};

void setCubeColour(scene::ISceneNode* node, video::SColor colour, u32 borderSize = 5) {
    video::ITexture* texture = node->getMaterial(0).getTexture(0);
    video::SColor* nodeTexturePixels = (video::SColor*)texture->lock(video::ETLM_WRITE_ONLY);
    for (u32 i = borderSize; i < texture->getSize().Height - borderSize; ++i) {
        for (u32 j = borderSize; j < texture->getSize().Width - borderSize; ++j) {
            nodeTexturePixels[i * texture->getSize().Height + j].set(colour.color);
        }
    }
    texture->unlock();
}

void createFugureXmlFile(IrrlichtDevice* device) {
    io::path fileName = "zigzag.xml";


    irr::io::IXMLWriter* xwriter = device->getFileSystem()->createXMLWriter(fileName);
    if (!xwriter) {
        irr::io::IWriteFile* file = device->getFileSystem()->createAndWriteFile(fileName);
        if (!file) {
            return;
        }
        
        xwriter = device->getFileSystem()->createXMLWriter(file);
        if (!xwriter) {
            return;
        }
    }    

    //write out the obligatory xml header. Each xml-file needs to have exactly one of those.
    xwriter->writeXMLHeader();

    //start element mygame, you replace the label "mygame" with anything you want
    xwriter->writeElement(L"figure", false, L"type", L"cube", L"comment", L"typical zigzag node");
    xwriter->writeLineBreak(); //new line

    //start section with video settings
    xwriter->writeElement(L"side", false, L"type", L"up");
    xwriter->writeLineBreak(); //new line
    
    xwriter->writeElement(L"side", false, L"type", L"left");
    xwriter->writeLineBreak(); //new line
    
    xwriter->writeClosingTag(L"side");
    xwriter->writeLineBreak();
    
    xwriter->writeClosingTag(L"side");
    xwriter->writeLineBreak();
    
    xwriter->writeElement(L"side", false, L"type", L"right");
    xwriter->writeLineBreak(); //new line
    
    xwriter->writeElement(L"side", false, L"type", L"right");
    xwriter->writeLineBreak(); //new line
    
    xwriter->writeClosingTag(L"side");
    xwriter->writeLineBreak();
    
    xwriter->writeClosingTag(L"side");
    xwriter->writeLineBreak();

    //close mygame section
    xwriter->writeClosingTag(L"figure");

    //delete xml writer
    xwriter->drop();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // Инициализируем наш виджет
    w.irrWidget.init();

    IrrlichtDevice* device = w.irrWidget.getIrrlichtDevice();
    ISceneManager *smgr = w.irrWidget.getIrrlichtDevice()->getSceneManager();
    IVideoDriver *driver = w.irrWidget.getIrrlichtDevice()->getVideoDriver();
    // Загружаем меш
    /*IAnimatedMesh* mesh = smgr->getMesh("/home/hokan/Irrlicht/irrlicht-1.8/media/sydney.md2");
    if (!mesh)
    {
        w.irrWidget.getIrrlichtDevice()->drop();
        return 1;
    }
    // Добавляем узел сцены, отключаем освещение, задаем анимацию и накладываем текстуру
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
    if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture( 0, driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/sydney.bmp") );
    }*/
    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

    scene::ICameraSceneNode* camera =
            smgr->addCameraSceneNodeFPS(0, 100.0f, 1.2f);

    camera->setPosition(core::vector3df(2700 * 2, 255 * 2, 2600 * 2));
    camera->setTarget(core::vector3df(2397 * 2, 343 * 2, 2700 * 2));
    camera->setFarValue(42000.0f);

    // disable mouse cursor
    //device->getCursorControl()->setVisible(false);

    /*
    Here comes the terrain renderer scene node: We add it just like any
    other scene node to the scene using
    ISceneManager::addTerrainSceneNode(). The only parameter we use is a
    file name to the heightmap we use. A heightmap is simply a gray scale
    texture. The terrain renderer loads it and creates the 3D terrain from
    it.

    To make the terrain look more big, we change the scale factor of
    it to (40, 4.4, 40). Because we don't have any dynamic lights in the
    scene, we switch off the lighting, and we set the file
    terrain-texture.jpg as texture for the terrain and detailmap3.jpg as
    second texture, called detail map. At last, we set the scale values for
    the texture: The first texture will be repeated only one time over the
    whole terrain, and the second one (detail map) 20 times.
     */

    // add terrain scene node
    scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
            "/home/hokan/Irrlicht/irrlicht-1.8/media/terrain-heightmap.bmp",
            0, // parent node
            -1, // node id
            core::vector3df(0.f, 0.f, 0.f), // position
            core::vector3df(0.f, 0.f, 0.f), // rotation
            core::vector3df(40.f, 4.4f, 40.f), // scale
            video::SColor(255, 255, 255, 255), // vertexColor
            5, // maxLOD
            scene::ETPS_17, // patchSize
            4 // smoothFactor
            );

    terrain->setMaterialFlag(video::EMF_LIGHTING, false);

    terrain->setMaterialTexture(0,
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/terrain-texture.jpg"));
    terrain->setMaterialTexture(1,
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/detailmap3.jpg"));

    terrain->setMaterialType(video::EMT_DETAIL_MAP);

    terrain->scaleTexture(1.0f, 20.0f);

    /*
    To be able to do collision with the terrain, we create a triangle selector.
    If you want to know what triangle selectors do, just take a look into the
    collision tutorial. The terrain triangle selector works together with the
    terrain. To demonstrate this, we create a collision response animator
    and attach it to the camera, so that the camera will not be able to fly
    through the terrain.
     */

    // create triangle selector for the terrain	
    scene::ITriangleSelector* selector
            = smgr->createTerrainTriangleSelector(terrain, 0);
    terrain->setTriangleSelector(selector);

    // create collision response animator and attach it to the camera
    scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
            selector, camera, core::vector3df(60, 100, 60),
            core::vector3df(0, 0, 0),
            core::vector3df(0, 50, 0));
    //camera->addAnimator(anim);
    anim->drop();


    /* If you need access to the terrain data you can also do this directly via the following code fragment.
     */
    /*scene::CDynamicMeshBuffer* buffer = new scene::CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
    terrain->getMeshBufferForLOD(*buffer, 0);
    video::S3DVertex2TCoords* data = (video::S3DVertex2TCoords*)buffer->getVertexBuffer().getData();
    // Work on data or get the IndexBuffer with a similar call.
    buffer->drop(); // When done drop the buffer again.*/

    /*
    To make the user be able to switch between normal and wireframe mode,
    we create an instance of the event receiver from above and let Irrlicht
    know about it. In addition, we add the skybox which we already used in
    lots of Irrlicht examples and a skydome, which is shown mutually
    exclusive with the skybox by pressing 'S'.
     */

    // create skybox and skydome
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

    scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_up.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_dn.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_lf.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_rt.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_ft.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_bk.jpg"));
    scene::ISceneNode* skydome = smgr->addSkyDomeSceneNode(driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/skydome.jpg"), 16, 8, 0.95f, 2.0f);

    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    /*IAnimatedMesh* mesh = smgr->getMesh("./media/models/cube.x");
    if (!mesh) {
        w.irrWidget.getIrrlichtDevice()->drop();
        return 1;
    }
    // Добавляем узел сцены, отключаем освещение, задаем анимацию и накладываем текстуру
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);*/
    IMeshSceneNode* node = smgr->addCubeSceneNode();    
    if (node) {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMaterialTexture(0, driver->getTexture("media/textures/cube.png"));
        setCubeColour(node, SColor(255, 255, 100, 100));
        node->setPosition(camera->getPosition() + 100);                
        node->setScale(core::vector3df(10.0f, 10.0f, 10.0f));
        //core::vector3df nodePosition = node->getPosition();
        /*node->addAnimator(smgr->createFlyStraightAnimator(
                nodePosition,
                core::vector3df(nodePosition.X, terrain->getPosition().Y, nodePosition.Z),
                10000));*/
        const core::aabbox3d<f32>& box = node->getBoundingBox();
        core::vector3df radius = box.MaxEdge - box.getCenter();
        radius *= 10;
        ISceneNodeAnimatorCollisionResponse* animator = smgr->createCollisionResponseAnimator(
                selector, node, radius,
                core::vector3df(0, -0.5, 0),
                -box.getCenter());

        selector->drop();
        node->addAnimator(animator);
        animator->drop();
        //node->setMD2Animation(scene::EMAT_STAND);
        //node->setMaterialTexture(0, driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/sydney.bmp"));
    }

    core::vector3df cameraPosition = camera->getPosition();

    /*CGridSceneNode* gridSceneNodeXZ = new CGridSceneNode(smgr->getRootSceneNode(), smgr, -1, 1);
    gridSceneNodeXZ->setGridsSize(irr::core::dimension2df(2500, 2500));
    gridSceneNodeXZ->getGrid(0).setSpacing(100.0f);
    gridSceneNodeXZ->getGrid(0).setGridColor(irr::video::SColor(255, 100, 255, 100));
    gridSceneNodeXZ->setPosition(core::vector3df(cameraPosition.X, terrain->getPosition().Y, cameraPosition.Z));
    gridSceneNodeXZ->drop(); // added to scene already, that still has a reference*/

    /*CGridSceneNode* gridSceneNodeYZ = new CGridSceneNode(smgr->getRootSceneNode(), smgr, -1, 1);
    gridSceneNodeYZ->setGridsSize(irr::core::dimension2df(2500, 2500));
    gridSceneNodeYZ->getGrid(0).setSpacing(100.f);
    gridSceneNodeYZ->getGrid(0).setGridColor(irr::video::SColor(255, 100, 255, 100));
    gridSceneNodeYZ->setPosition(core::vector3df(cameraPosition.X, terrain->getPosition().Y + 1250, cameraPosition.Z - 1250));
    core::vector3df newAngle = gridSceneNodeYZ->getRotation();
    //newAngle.rotateYZBy(90.0f, core::vector3df(cameraPosition.X, 0, 0));
    newAngle.X += 90.0f;
    gridSceneNodeYZ->setRotation(newAngle);
    gridSceneNodeYZ->drop(); // added to scene already, that still has a reference*/
    SGridBox gridBox(smgr->getRootSceneNode(), smgr);
    gridBox.setGridsSize(core::dimension2df(2500.0f, 500.0f));
    //gridBox.setGridsSize(core::dimension2df(510.0f, 500.0f));
    gridBox.setColour(irr::video::SColor(255, 100, 255, 100));
    gridBox.setPosition(core::vector3df(cameraPosition.X, terrain->getPosition().Y, cameraPosition.Z));
    gridBox.setSpacing(100.0f);
    gridBox.turnOn2DMode();

    // create event receiver
    MyEventReceiver receiver(terrain, skybox, skydome, node);
    device->setEventReceiver(&receiver);
    
    //createFugureXmlFile(device);
    
    CFigure figure(smgr->getRootSceneNode(), smgr, -1, device->getFileSystem(), 10.0f, io::path("zigzag.xml"));
    //figure.createFigureFromFile(io::path("cube.xml"));        
    figure.applyTextureToFigure("media/textures/cube.png", video::SColor(255, 100, 255, 100));    
    //figure.setPosition(cameraPosition);
    figure.setDebugDataVisible(scene::EDS_BBOX);
    //figure.saveFigureToFile("zigzag1.xml");
    
    //video::SMaterial material;
    //material.EmissiveColor.set(255, 255, 100, 100);
    //node->getMesh()->getMeshBuffer(0)->getMaterial() = material; //.DiffuseColor.set(255, 255, 100, 100);
    //smgr->getMeshManipulator()->setVertexColors(node->getMesh(), SColor(255, 255, 100, 100));

    return a.exec();
}
