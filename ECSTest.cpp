#define BOOST_TEST_MODULE ECSTest

#include <boost/test/included/unit_test.hpp>

#include <boost/test/unit_test.hpp>
#include "PackedArray.h"
#include "ComponentSet.h"
#include "ComponentManager.h"
#include "EntityManager.h" 
#include <bitset>
BOOST_AUTO_TEST_SUITE(ECSTest)

BOOST_AUTO_TEST_CASE(TestAddItem) {
	PackedArray<int> packedArray;
	int item = 42;
	packedArray.Add(item);

	// Verify that the item is added and can be retrieved
	int* retrievedItem = packedArray.Get(0);
	BOOST_TEST(*retrievedItem == item);
	BOOST_TEST(packedArray.Count == 1);
}

BOOST_AUTO_TEST_CASE(TestRemoveItem) {
	PackedArray<int> packedArray;
	int item1 = 42;
	int item2 = 100;
	packedArray.Add(item1);
	packedArray.Add(item2);

	// Remove an item from the packed array
	packedArray.Remove(0);

	// Verify that the item is removed and the count is updated
	BOOST_TEST(packedArray.Count == 1);
	BOOST_TEST(*packedArray.Get(0) == item2);
}

BOOST_AUTO_TEST_CASE(TestGetEmpty) {
	PackedArray<int> packedArray;

	int* retreivedItem = packedArray.Get(0);
	// Verify that the retreived item results in a nullptr
	BOOST_TEST(retreivedItem == nullptr);
}

BOOST_AUTO_TEST_CASE(TestAddEntity)
{
	unsigned short entityId = 0;
	unsigned short entityId2 = 42;
	ComponentSet<int> intComponents(0);

	intComponents.AddEntity(entityId);
	intComponents.AddEntity(entityId2);
	BOOST_TEST(intComponents.count == 2);
	BOOST_TEST(intComponents.HasEntity(entityId));
	BOOST_TEST(intComponents.HasEntity(entityId2));
}

BOOST_AUTO_TEST_CASE(TestGetComponent)
{
	unsigned short entityId = 42;
	ComponentSet<int> intComponents(0);

	intComponents.AddEntity(entityId);
	int* result = intComponents.GetComponent(entityId);

	BOOST_TEST(result != nullptr);
}

BOOST_AUTO_TEST_CASE(TestGetNoComponent)
{
	unsigned short entityId = 42;
	ComponentSet<int> intComponents(0);

	int* result = intComponents.GetComponent(entityId);

	BOOST_TEST(result == nullptr);
}

BOOST_AUTO_TEST_CASE(TestHasEntity)
{
	unsigned short entityId = 0;
	unsigned short entityId2 = 42;
	ComponentSet<int> intComponents(0);

	intComponents.AddEntity(entityId);
	BOOST_TEST(intComponents.HasEntity(entityId) == TRUE);
	BOOST_TEST(intComponents.HasEntity(entityId2) == FALSE);
}

BOOST_AUTO_TEST_CASE(TestRemoveOneEntity)
{
	unsigned short entityId = 0;
	ComponentSet<int> intComponents(0);

	intComponents.AddEntity(entityId);

	intComponents.RemoveEntity(entityId);

	BOOST_TEST(intComponents.count == 0);
	BOOST_TEST(intComponents.HasEntity(entityId) == FALSE);
}

BOOST_AUTO_TEST_CASE(TestRemoveEntity)
{
	unsigned short entityId = 0;
	unsigned short entityId2 = 42;
	ComponentSet<int> intComponents(0);

	//intComponents.AddEntity(entityId);
	intComponents.AddEntity(entityId2);

	intComponents.RemoveEntity(entityId);

	BOOST_TEST(intComponents.count == 1);
	BOOST_TEST(intComponents.HasEntity(entityId) == FALSE);
	BOOST_TEST(intComponents.HasEntity(entityId2) == TRUE);
}

BOOST_AUTO_TEST_CASE(TestCreateComponentSet) {
	ComponentSet<int>* result = ComponentManager::CreateComponentSet<int>();

	std::string resultType = result->ComponentName();
	BOOST_TEST(resultType._Equal("int"));
	BOOST_TEST(result != nullptr);
	BOOST_TEST(ComponentManager::GetComponentKey<int>() == 0);
}

BOOST_AUTO_TEST_CASE(TestGetComponentSet) {
	ComponentManager::CreateComponentSet<int>();

	ComponentSet<int>* result = ComponentManager::GetComponentSet<int>();
	std::string resultType = result->ComponentName();
	BOOST_TEST(ComponentManager::componentKeys["int"] == 0);
	BOOST_TEST(resultType._Equal("int"));
	BOOST_TEST(result != nullptr);
}

BOOST_AUTO_TEST_CASE(TestGetComponentKey) {
	ComponentManager::CreateComponentSet<int>();
	BOOST_TEST(ComponentManager::GetComponentKey<int>() == 0);
}

BOOST_AUTO_TEST_CASE(TestBitHas) {
	// ...0000 0000 0000 1100
	BitTracker tracker = BitTracker(0b1000);
	int index = 3;
	int index2 = 2;
	bool result = tracker.Has(index);
	BOOST_TEST(result == true);
	BOOST_TEST(tracker.Has(index2) == false);
}

BOOST_AUTO_TEST_CASE(TestBitAdd) {
	BitTracker tracker = BitTracker();
	int index1 = 1;
	tracker.Add(index1);
	
	BOOST_TEST(tracker.Has(index1) == true);
	BOOST_TEST(tracker.bits == 0b0010);
}

BOOST_AUTO_TEST_CASE(TestBitRemove) {
	BitTracker tracker = BitTracker(0b0100);
	tracker.Remove(2);
	BOOST_TEST(tracker.Has(2) == false);
	BOOST_TEST(tracker.bits == 0b0000);
}

BOOST_AUTO_TEST_CASE(TestManagerCreateEntity) {
	Entity entity = EntityManager::CreateEntity();
	
	BOOST_TEST(EntityManager::activeEntityBits[entity._id].bits == 0);
	BOOST_TEST(EntityManager::entityCount == 1);
	BOOST_TEST(entity._id == 0);
}

BOOST_AUTO_TEST_CASE(TestManagerAddComponentKey) {
	Entity entity = EntityManager::CreateEntity();
	EntityManager::AddComponentKey(&entity, 1);

	BOOST_TEST(EntityManager::activeEntityBits[entity._id].bits == 0b0010);
}

BOOST_AUTO_TEST_CASE(TestManagerRemoveComponentKey) {
	Entity entity = EntityManager::CreateEntity();
	EntityManager::AddComponentKey(&entity, 1);
	EntityManager::RemoveComponentKey(&entity, 1);

	BOOST_TEST(EntityManager::activeEntityBits[entity._id].bits == 0b0000);
}

BOOST_AUTO_TEST_CASE(TestManagerDestroyEntity) {
	Entity entity = EntityManager::CreateEntity();
	EntityManager::DestroyEntity(&entity);
	BOOST_TEST(EntityManager::deadEntities.Count == 1);
	BOOST_TEST(EntityManager::entityCount == 0);
}

BOOST_AUTO_TEST_CASE(TestManagerDestroyEntityWithComponents) {
	Entity entity = EntityManager::CreateEntity();
	ComponentManager::CreateComponentSet<int>()->AddEntity(entity._id);
	EntityManager::DestroyEntity(&entity);
	BOOST_TEST(EntityManager::deadEntities.Count == 1);
	BOOST_TEST(EntityManager::entityCount == 0);
	BOOST_TEST(EntityManager::activeEntityBits[entity._id].bits == 0);
}

BOOST_AUTO_TEST_CASE(TestEntityHasComponent) {
	Entity testEntity = EntityManager::CreateEntity();
	ComponentManager::CreateComponentSet<int>()->AddEntity(testEntity._id);
	ComponentManager::CreateComponentSet<char>();
	BOOST_TEST(testEntity.HasComponent<int>() == true);
	BOOST_TEST(testEntity.HasComponent<std::string>() == false);
	BOOST_TEST(testEntity.HasComponent<char>() == false);
}

BOOST_AUTO_TEST_CASE(TestEntityAddComponent) {
	Entity testEntity = EntityManager::CreateEntity();
	ComponentSet<int>* componentSet = ComponentManager::CreateComponentSet<int>();
	testEntity.AddComponent<int>();
	BOOST_TEST(componentSet->HasEntity(testEntity._id) == true);
	BOOST_TEST(EntityManager::activeEntityBits[testEntity._id].bits == 0b0001);
}

BOOST_AUTO_TEST_CASE(TestEntityRemoveComponent) {
	Entity testEntity = EntityManager::CreateEntity();
	ComponentSet<int>* componentSet = ComponentManager::CreateComponentSet<int>();
	testEntity.AddComponent<int>();
	testEntity.RemoveComponent<int>();
	testEntity.RemoveComponent<std::string>();
	BOOST_TEST(componentSet->HasEntity(testEntity._id) == false);
	BOOST_TEST(EntityManager::activeEntityBits[testEntity._id].bits == 0b0000);
}

BOOST_AUTO_TEST_CASE(TestEntityGetComponent) {
	Entity testEntity = EntityManager::CreateEntity();
	ComponentSet<int>* componentSet = ComponentManager::CreateComponentSet<int>();
	testEntity.AddComponent<int>();
	std::string* component = testEntity.GetComponent<std::string>();
	component->assign("Hello World!");
	BOOST_TEST(component->compare("Hello World!") == 0);
	BOOST_TEST(component->empty() == false);
}


BOOST_AUTO_TEST_SUITE_END()
