#include "Engine.h"
#include <iostream>

Engine::Engine()
{
}

Engine::~Engine()
{
}

void LoadCube(Mesh& cube) {

    cube.Name = "Cube";
    cube.AddVertices(Vector3d(0.0f, 0.0f, 0.0f)); //0
    cube.AddVertices(Vector3d(0.0f, 1.0f, 0.0f)); //1
    cube.AddVertices(Vector3d(1.0f, 1.0f, 0.0f)); //2
    cube.AddVertices(Vector3d(1.0f, 0.0f, 0.0f)); //3

    cube.AddVertices(Vector3d(1.0f, 1.0f, 1.0f)); //4
    cube.AddVertices(Vector3d(1.0f, 0.0f, 1.0f)); //5
    cube.AddVertices(Vector3d(0.0f, 1.0f, 1.0f)); //6
    cube.AddVertices(Vector3d(0.0f, 0.0f, 1.0f)); //7

    cube.AddFace(0, 1, 2);
    cube.AddFace(0, 2, 3);

    cube.AddFace(3, 2, 4);
    cube.AddFace(3, 4, 5);

    cube.AddFace(5, 4, 6);
    cube.AddFace(5, 6, 7);

    cube.AddFace(7, 6, 1);
    cube.AddFace(7, 1, 0);

    cube.AddFace(1, 6, 4);
    cube.AddFace(1, 4, 2);

    cube.AddFace(5, 7, 0);
    cube.AddFace(5, 0, 3);

}

void Engine::Start()
{
    Scene scene = Scene();
    Mesh myMyesh = Mesh();
    ModelLoader ml = ModelLoader();
   // ml.LoadModel("../Models/cube.obj", myMyesh, ModelLoader::FileType::OBJ);
    ml.LoadModel("../Models/ring.dae", myMyesh, ModelLoader::FileType::DAE);
    //ml.LoadModel("../Models/suzan.obj", myMyesh, ModelLoader::FileType::OBJ);
   // ml.LoadModel("../Models/teapot.obj", myMyesh, ModelLoader::FileType::OBJ);
    //LoadCube(myMyesh);
    scene.Objects.push_back(myMyesh);
    
    Renderer render = Renderer();
   
	sf::RenderWindow okno(sf::VideoMode(900,900), "HeroEngine");

    okno.clear();

    render.Render(scene, okno);

    okno.display();
    
	while (okno.isOpen()) {

        sf::Event event;
        while (okno.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                okno.close();

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Escape) {
                    okno.close();
                }

                if (event.key.code == sf::Keyboard::Right) {
                    scene.Objects[0].Position.x += 0.5f;
                }

                if (event.key.code == sf::Keyboard::Left) {
                    scene.Objects[0].Position.x -= 0.5f;
                }

                if (event.key.code == sf::Keyboard::Up) {
                    scene.Objects[0].Position.z += 0.5f;
                }

                if (event.key.code == sf::Keyboard::Down) {
                    scene.Objects[0].Position.z -= 0.5f;
                }

                if (event.key.code == sf::Keyboard::U) {
                    scene.Objects[0].Position.y += 0.5f;
                }

                if (event.key.code == sf::Keyboard::J) {
                    scene.Objects[0].Position.y -= 0.5f;
                }

                if (event.key.code == sf::Keyboard::Y) {
                    scene.Objects[0].Rotation.x += 0.05f;
                }

                if (event.key.code == sf::Keyboard::H) {
                    scene.Objects[0].Rotation.y += 0.05f;
                }

                if (event.key.code == sf::Keyboard::N) {
                    scene.Objects[0].Rotation.z += 0.05f;
                }

                if (event.key.code == sf::Keyboard::T) {
                    scene.Objects[0].Rotation.x -= 0.05f;
                }

                if (event.key.code == sf::Keyboard::G) {
                    scene.Objects[0].Rotation.y -= 0.05f;
                }

                if (event.key.code == sf::Keyboard::B) {
                    scene.Objects[0].Rotation.z -= 0.05f;
                }

                if (event.key.code == sf::Keyboard::F) {
                    render.Render(scene, okno, true);
                }

                if (event.key.code == sf::Keyboard::W) {
                    scene.Cam.position.Add(scene.Cam.lookDir);
                }

                if (event.key.code == sf::Keyboard::S) {
                    scene.Cam.position.Sub(scene.Cam.lookDir);
                }

                if (event.key.code == sf::Keyboard::D) {
                    scene.Cam.position.Add(Vector3d::CrossProduct(scene.Cam.lookDir,scene.Cam.upDirection));
                }

                if (event.key.code == sf::Keyboard::A) {
                    scene.Cam.position.Sub(Vector3d::CrossProduct(scene.Cam.lookDir, scene.Cam.upDirection));
                }

                if (event.key.code == sf::Keyboard::LShift) {
                    scene.Cam.position.Add(scene.Cam.upDirection);
                }

                if (event.key.code == sf::Keyboard::LControl) {
                    scene.Cam.position.Sub(scene.Cam.upDirection);
                }

                if (event.key.code == sf::Keyboard::E) {
                    scene.Cam.Yaw += 0.05f;
                }

                if (event.key.code == sf::Keyboard::Q) {
                    scene.Cam.Yaw -= 0.05f;
                }
                
                okno.clear();

                render.Render(scene, okno);

                okno.display();
            }

        } //while

        

        
	}

    
}
