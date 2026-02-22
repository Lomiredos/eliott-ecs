# ENGINE MASTER — Fichier de suivi projet
> Dernière mise à jour : 2026-02-21
> SDL3 | C++20 | CMake | Git Submodules | vcpkg

---

## 🤖 CONTEXTE POUR CLAUDE CODE

### Qui est l'utilisateur
Eliott, étudiant GTECH1 à Gaming Campus Lyon. Il apprend le développement de moteur de jeu en C++.

### Règles pédagogiques IMPORTANTES
- **Ne jamais générer du code complet** sauf si Eliott dit explicitement "code-moi ça", "écris le code", "génère la fonction"
- À la place : guider par étapes, donner la structure avec commentaires, poser des questions
- Pointer les erreurs avec précision (numéro de ligne), expliquer POURQUOI, donner des indices pas la correction
- Forcer un codage générique et durable
- Poser des questions de clarification avant de répondre

### État d'avancement au moment du transfert
- Architecture globale : ✅ définie
- Setup VS Code + CMake + vcpkg : ✅ fonctionnel
- Repo `eliott-ecs` : ✅ créé localement (`C:\Dev\eliott-ecs`)
- **Tâche en cours** : eliott-ecs terminé ✅ — prochaine étape : eliott-tmx-parser

### Ce qu'Eliott a déjà compris (bases théoriques validées)
- ECS : Entity = juste un uint32_t (ID), Component = data only, System = logique
- Stockage par tableau par type de component (approche cache-friendly)
- `unordered_map<EntityID, size_t>` pour l'index
- Recyclage des IDs via `std::queue` + `std::unordered_set` pour les vivants
- La queue est vide au départ, on prend `_nextID` si vide, sinon on pop

---

## 🗺️ Vue d'ensemble architecture

```
[game-project]          ← Repo utilisateur final (jeu)
      │
      └── [engine]      ← Repo moteur principal
              ├── [ecs]          ← submodule
              ├── [tmx-parser]   ← submodule
              ├── [renderer]     ← submodule
              ├── [audio]        ← submodule
              ├── [input]        ← submodule
              └── [physics]      ← submodule
```

---

## 📦 Repos & responsabilités

### 1. `eliott-ecs`
- **Rôle** : Système ECS générique, réutilisable par n'importe quel projet
- **Dépendances externes** : doctest (tests uniquement)
- **Expose** : `World`, `Entity`, `ComponentManager`, `SystemManager`
- **Standard** : C++20, lib statique
- **Status** : ✅ Terminé

### 2. `eliott-tmx-parser`
- **Rôle** : Parser de fichiers `.tmx` (Tiled Map Editor)
- **Dépendances externes** : tinyxml2 (déjà utilisé dans RuneBorn)
- **Expose** : `TmxMap`, `TmxLayer`, `TmxTileset`, `TmxObject`
- **Note** : Une version existe dans RuneBorn (`TiledParser.h`) à refactoriser
- **Status** : 🔴 À créer

### 3. `eliott-renderer`
- **Rôle** : Abstraction du rendu 2D via SDL3
- **Dépendances externes** : SDL3
- **Expose** : `Renderer`, `Texture`, `Camera`, `SpriteBatch`
- **Dépend de** : `eliott-ecs`
- **Status** : 🔴 À créer

### 4. `eliott-audio`
- **Rôle** : Gestion des sons et musiques
- **Dépendances externes** : SDL3_mixer ou miniaudio (à choisir)
- **Expose** : `AudioManager`, `Sound`, `Music`
- **Status** : 🔴 À créer

### 5. `eliott-input`
- **Rôle** : Gestion des entrées clavier/souris/gamepad
- **Dépendances externes** : SDL3
- **Expose** : `InputManager`, `ActionMap`, `KeyBinding`
- **Status** : 🔴 À créer

### 6. `eliott-physics`
- **Rôle** : Physique rigide 2D + détection via Quadtree
- **Dépendances externes** : aucune
- **Expose** : `PhysicsWorld`, `RigidBody`, `Quadtree`, `Collider`
- **Dépend de** : `eliott-ecs`
- **Status** : 🔴 À créer

### 7. `eliott-engine`
- **Rôle** : Moteur principal, assemble tous les modules
- **Dépendances** : tous les submodules + SDL3
- **Expose** : `Engine`, `Scene`, `SceneManager`, `GameLoop`
- **Status** : 🔴 À créer

---

## 🔗 Matrice de dépendances inter-modules

| Module       | ecs | tmx | renderer | audio | input | physics |
|--------------|:---:|:---:|:--------:|:-----:|:-----:|:-------:|
| ecs          |  —  |     |          |       |       |         |
| tmx-parser   |     |  —  |          |       |       |         |
| renderer     |  ✓  |  ✓  |    —     |       |       |         |
| audio        |     |     |          |   —   |       |         |
| input        |     |     |          |       |   —   |         |
| physics      |  ✓  |     |          |       |       |    —    |
| **engine**   |  ✓  |  ✓  |    ✓     |   ✓   |   ✓   |    ✓    |

---

## 🗂️ Structure type de chaque repo

```
eliott-<module>/
├── CMakeLists.txt
├── README.md
├── include/
│   └── <module>/
│       └── *.hpp          ← API publique
├── src/
│   └── *.cpp
└── tests/
    └── test_*.cpp
```

---

## 🗂️ Structure actuelle `eliott-ecs`

```
C:\Dev\eliott-ecs\
├── CMakeLists.txt              ✅
├── include/
│   └── ecs/
│       ├── ComponentRegistry.hpp   ✅ (Signature, EntityID, getComponentID<T>)
│       ├── EntityManager.hpp       ✅
│       ├── ComponentArray.hpp      ✅ (IComponentArray + ComponentArray<T>)
│       ├── ComponentManager.hpp    ✅
│       ├── System.hpp              ✅
│       ├── SystemManager.hpp       ✅
│       └── World.hpp               ✅ (façade)
├── src/
│   └── EntityManager.cpp           ✅
└── tests/
    ├── test_entity.cpp             ✅
    ├── test_component.cpp          ✅
    ├── test_componentManager.cpp   ✅
    ├── test_systemManager.cpp      ✅
    └── test_world.cpp              ✅
```

### CMakeLists.txt actuel (fonctionnel)
```cmake
cmake_minimum_required(VERSION 3.20)
project(eliott-ecs VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "C:/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "C:/Dev/vcpkg/installed/x64-windows")

find_package(doctest CONFIG REQUIRED)

add_library(eliott-ecs STATIC
    src/EntityManager.cpp
)

target_include_directories(eliott-ecs PUBLIC
    include/
)

add_executable(tests
    tests/test_entity.cpp
)

target_link_libraries(tests PRIVATE
    eliott-ecs
    doctest::doctest
)
```

### EntityManager.hpp actuel
```cpp
#pragma once
#include <cstdint>
#include <queue>
#include <unordered_set>

namespace ee::ecs {

using EntityID = uint32_t;

class EntityManager {
public:
    EntityID createEntity();
    void destroyEntity(EntityID id);
    bool isAlive(EntityID id) const;
    size_t getEntityCount() const;

private:
    std::queue<EntityID>         _availableIDs;
    std::unordered_set<EntityID> _livingEntities;
    uint32_t                     _nextID = 0;
};

} // namespace ee::ecs
```

---

## ✅ Ordre de développement

```
Phase 1 — Fondations
  ✅ eliott-ecs
    ✅ EntityManager
    ✅ ComponentArray<T> + IComponentArray
    ✅ ComponentManager
    ✅ SystemManager
    ✅ World (façade)
    ✅ Tests doctest (tous passent)
  🔴 eliott-tmx-parser     ← PROCHAINE ÉTAPE

Phase 2 — Modules SDL3
  🔴 eliott-input
  🔴 eliott-audio

Phase 3 — Modules ECS-dépendants
  🔴 eliott-renderer
  🔴 eliott-physics (Quadtree + RigidBody)

Phase 4 — Assemblage
  🔴 eliott-engine

Phase 5 — Validation
  🔴 Mini-jeu de test avec map Tiled
```

---

## 🔧 Conventions de code

- Namespace : `ee::ecs::`, `ee::physics::`, `ee::renderer::`, etc.
- Membres privés préfixés `_` : `_availableIDs`, `_nextID`
- Pas de `using namespace` dans les headers
- RAII strict, pas de `new`/`delete` raw
- `std::optional` pour les retours faillibles
- Interfaces → classe abstraite pure ou concept C++20

---

## 📋 Décisions actées

| Sujet | Décision |
|-------|----------|
| SDL | SDL3 |
| Build | CMake |
| Dépendances | vcpkg |
| Tests | doctest |
| Lib | Statique (.lib) |
| ECS RuneBorn | Refactorisé from scratch |
| Stockage components | unordered_map<EntityID, T> par type |
| Recyclage IDs | std::queue + std::unordered_set |

## 📋 Décisions en suspens

| Décision | Options |
|----------|---------|
| Backend audio | SDL3_mixer / miniaudio |

---

## 📌 Notes de session

```
[2026-02-21] Session 1 — Initialisation complète du projet.
             Architecture définie, setup CMake+vcpkg fonctionnel.
             Bases théoriques ECS validées avec Eliott.
             EntityManager.hpp écrit, .cpp à implémenter par Eliott.

[2026-02-21] Session 2 — eliott-ecs complété.
             Implémenté : ComponentArray<T>, IComponentArray, ComponentManager,
             ComponentRegistry (Signature/getComponentID), System, SystemManager, World.
             Tous les tests passent (doctest).
             Concepts clés vus : type erasure, std::type_index, std::static_pointer_cast,
             inline variables (ODR), bitmask signatures, façade pattern.
             Prochaine session : eliott-tmx-parser.
```