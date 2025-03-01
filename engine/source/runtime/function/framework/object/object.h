#pragma once

#include "runtime/function/framework/component/component.h"
#include "runtime/function/framework/object/object_id_allocator.h"

#include "runtime/resource/res_type/common/object.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace Pilot
{
    /// GObject : Game Object base class
    class GObject
    {
        typedef std::unordered_set<std::string> TypeNameSet;

    public:
        GObject(GObjectID id) : m_id {id} {}
        virtual ~GObject();

        virtual void tick(float delta_time);

        bool load(const ObjectInstanceRes& object_instance_res);
        void save(ObjectInstanceRes& out_object_instance_res);

        bool loadComponents(const std::vector<std::string>& components, TypeNameSet& out_instance_component_type_set);

        bool loadComponentDefinition(const ComponentDefinitionRes& component_definition_res,
                                     const bool                    is_instance_component,
                                     TypeNameSet&                  out_instance_component_type_set);

        GObjectID getID() const { return m_id; }

        void               setName(std::string name) { m_name = name; }
        const std::string& getName() const { return m_name; }

        bool hasComponent(const std::string& compenent_type_name) const
        {
            for (const std::string& type_name : m_component_type_names)
            {
                if (compenent_type_name == type_name)
                    return true;
            }
            return false;
        }

        std::vector<Reflection::ReflectionPtr<Component>> getComponents() { return m_components; }

        template<typename TComponent>
        TComponent* tryGetComponent(const std::string& compenent_type_name)
        {
            for (int i = 0; i < m_components.size(); ++i)
            {
                if (compenent_type_name == m_component_type_names[i])
                {
                    return static_cast<TComponent*>(m_components[i].operator->());
                }
            }
            return nullptr;
        }

        template<typename TComponent>
        const TComponent* tryGetComponentConst(const std::string& compenent_type_name) const
        {
            for (int i = 0; i < m_components.size(); ++i)
            {
                if (compenent_type_name == m_component_type_names[i])
                {
                    return static_cast<TComponent*>(m_components[i].operator->());
                }
            }
            return nullptr;
        }

#define tryGetComponent(COMPONENT_TYPE) tryGetComponent<COMPONENT_TYPE>(#COMPONENT_TYPE)
#define tryGetComponentConst(COMPONENT_TYPE) tryGetComponentConst<const COMPONENT_TYPE>(#COMPONENT_TYPE)

    protected:
        GObjectID   m_id {k_invalid_gobject_id};
        std::string m_name;
        std::string m_definition_url;

        std::vector<Reflection::ReflectionPtr<Component>> m_components;
        std::vector<std::string>                          m_component_type_names;
    };
} // namespace Pilot
