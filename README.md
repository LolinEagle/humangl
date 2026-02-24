# HumanGL
This project will show a 3D object conceived with a modelization program like Blender. In a window, the 3D object will be displayed in perspective, rotate on itself around its main symmetrical axis. The object can be moved on three axis, in both directions. Finally, a texture can be applied simply with the object when we press a dedicated key, and the same key allows us to go back. This project also implements a custom matrix stack and transformation system to perform skeletal animation. The goal is to create a hierarchical model where body parts are articulated and move logically.

<img src="https://github.com/user-attachments/assets/02cd1565-14ef-473a-b1a4-ee119dacfccf" width="400" height="225"/>
<img src="https://github.com/user-attachments/assets/17d84c10-9cb9-4c7a-b6f6-a24fc4a96793" width="400" height="225"/>
<img src="https://github.com/user-attachments/assets/362c8eaf-8cc6-41da-9097-84ccd7ab9b49" width="400" height="225"/>
<img src="https://github.com/user-attachments/assets/cdef6286-8273-4e19-9e8c-3e08ea5f29f1" width="400" height="225"/>

## Model Structure
The 3D model consists of the following hierarchical parts :

<img src="https://github.com/user-attachments/assets/75ef6c8e-105c-45c2-9684-fd9febeeb7f1" width="840" height="410" />

## Deployment
To deploy this project run
```bash
git clone https://github.com/LolinEagle/humangl && cd humangl && make run
```

## Arguments
```bash
./humangl <scene> [color] [texture]
```
scene : A number to choose a premade scene

color : (optional) A number to choose default color for polygon

texture : (optional) A number to choose texture object

## Related projects
Here are some related projects

[Cub3D](https://github.com/LolinEagle/Cub3D)

[So Long](https://github.com/LolinEagle/so_long)
