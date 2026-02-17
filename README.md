Implementación Entity-Component-System en C++

Características principales:
* Creación y destrucción de entidades con ids únicos pero reciclables.
* Signatures (bitsets) asociados a cada entidad para determinar tenencia de componentes y destrucción.
* Registro y manejo general (add/get/remove) de componentes con uso de sparse sets (dense y sparse)
* Sistemas iteran pools densos

## Demo básica de demostración usando ECS como API

(Requiere de Raylib: https://www.raylib.com/)

```bash
.
├── demo
│   ├── components.hpp
│   ├── main.cpp
│   ├── systems.cpp
│   └── systems.hpp
```

Compilación:
```bash
make build
```
Ejecución:
```bash
make run
```
