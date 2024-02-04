#include "Entity.h"

Entity::Entity(const size_t & id, const std::string & tag) : m_id(id), m_tag(tag) {
    // Costruttore
}

void Entity::destroy() {
    // Implementazione del metodo destroy
    m_active = false;
}

size_t Entity::id() const {
    // Implementazione del metodo id
    return m_id;
}

bool Entity::isActive() const {
    // Implementazione del metodo isActive
    return m_active;
}

const std::string & Entity::tag() const {
    // Implementazione del metodo tag
    return m_tag;
}