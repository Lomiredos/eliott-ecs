#pragma once
#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace ee::ecs {
	class World {

	private:

		ComponentManager m_compManager;
		EntityManager m_entityManager;
		SystemManager m_sysManager;
		std::unordered_map<EntityID, Signature> m_signatures;

		std::vector<EntityID> m_entitiesToAdd;
		std::vector<EntityID> m_entitiesToDestroy;
		std::unordered_set<EntityID> m_pendingEntities;

	public:
		EntityID createEntity() {
			EntityID id = m_entityManager.reserveID();
			m_entitiesToAdd.push_back(id);
			m_pendingEntities.insert(id);
			return id;
		}

		void destroyEntity(EntityID _id) {
			m_entitiesToDestroy.push_back(_id);
		}

		void flush() {
			for (EntityID id : m_entitiesToAdd) {
				m_entityManager.activateEntity(id);
				m_sysManager.onEntitySignatureChanged(id, m_signatures[id]);
			}
			m_entitiesToAdd.clear();
			m_pendingEntities.clear();

			for (EntityID id : m_entitiesToDestroy) {
				m_sysManager.onEntityDestroyed(id);
				m_compManager.onEntityDestroyed(id);
				m_entityManager.DestroyEntity(id);
				m_signatures.erase(id);
			}
			m_entitiesToDestroy.clear();
		}

		template<typename T>
		void addComponent(EntityID _id, T _comp) {
			m_compManager.addComponent(_id, _comp);
			m_signatures[_id].set(getComponentID<T>());
			if (!m_pendingEntities.count(_id))
				m_sysManager.onEntitySignatureChanged(_id, m_signatures[_id]);
		}

		template<typename T>
		void removeComponent(EntityID _id) {
			m_compManager.removeComponent<T>(_id);
			m_signatures[_id].reset(getComponentID<T>());
			m_sysManager.onEntitySignatureChanged(_id, m_signatures[_id]);
		}

		template<typename T>
		T& getComponent(EntityID _id) {

			return m_compManager.getComponent<T>(_id);
		}

		template<typename T>
		bool hasComponent(EntityID _id) {
			return m_compManager.hasComponent<T>(_id);
		}

		template<typename T>
		std::shared_ptr<T> registerSystem() {

			return m_sysManager.registerSystem<T>();
		}

		template<typename T>
		void setSystemSignature(Signature _newSign) {

			m_sysManager.setSignature<T>(_newSign);
		}

	};

}