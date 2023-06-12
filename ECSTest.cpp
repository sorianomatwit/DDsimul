#define BOOST_TEST_MODULE ECSTest

#include <boost/test/included/unit_test.hpp>

#include <boost/test/unit_test.hpp>
#include "PackedArray.h"
#include "ComponentSet.h"
#include "ComponentManager.h"
#include "EntityManager.h" 
#include <bitset>
BOOST_AUTO_TEST_SUITE(ECSTest)

BOOST_AUTO_TEST_CASE(TestPackedArray_AddItem) {
	PackedArray<int> packedArray;
	int item = 42;
	packedArray.Add(item);

	// Verify that the item is added and can be retrieved
	int* retrievedItem = packedArray.Get(0);
	BOOST_TEST(*retrievedItem == item);
	BOOST_TEST(packedArray.Count == 1);
}

BOOST_AUTO_TEST_CASE(TestPackedArray_RemoveItem) {
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

BOOST_AUTO_TEST_CASE(TestPackedArray_GetEmpty) {
	PackedArray<int> packedArray;

	int* retreivedItem = packedArray.Get(0);
	// Verify that the retreived item results in a nullptr
	BOOST_TEST(retreivedItem == nullptr);
}

BOOST_AUTO_TEST_CASE(TestComponentSet_AddEntity)
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

BOOST_AUTO_TEST_CASE(TestComponentSet_GetComponent)
{
	unsigned short entityId = 42;
	ComponentSet<int> intComponents(0);

	intComponents.AddEntity(entityId);
	int* result = intComponents.GetComponent(entityId);

	BOOST_TEST(result != nullptr);
}

BOOST_AUTO_TEST_CASE(TestComponentSet_GetNoComponent)
{
	unsigned short entityId = 42;
	ComponentSet<int> intComponents(0);

	int* result = intComponents.GetComponent(entityId);

	BOOST_TEST(result == nullptr);
}

BOOST_AUTO_TEST_CASE(TestComponentSet_HasEntity)
{
	unsigned short entityId = 0;
	unsigned short entityId2 = 42;
	ComponentSet<int> intComponents(0);

	intComponents.AddEntity(entityId);
	BOOST_TEST(intComponents.HasEntity(entityId) == TRUE);
	BOOST_TEST(intComponents.HasEntity(entityId2) == FALSE);
}

BOOST_AUTO_TEST_CASE(TestComponentSet_RemoveOneEntity)
{
	unsigned short entityId = 0;
	ComponentSet<int> intComponents(0);

	intComponents.AddEntity(entityId);

	intComponents.RemoveEntity(entityId);

	BOOST_TEST(intComponents.count == 0);
	BOOST_TEST(intComponents.HasEntity(entityId) == FALSE);
}

BOOST_AUTO_TEST_CASE(TestComponentSet_RemoveEntity)
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

BOOST_AUTO_TEST_CASE(TestComponentManager_CreateComponentSet) {
	ComponentSet<int>* result = ComponentManager::CreateComponentSet<int>();

	std::string resultType = result->ComponentName();
	BOOST_TEST(resultType._Equal("int"));
	BOOST_TEST(result != nullptr);
	BOOST_TEST(ComponentManager::GetComponentKey<int>() == 0);
}

BOOST_AUTO_TEST_CASE(TestComponentManager_GetComponentSet) {
	ComponentManager::CreateComponentSet<int>();

	ComponentSet<int>* result = ComponentManager::GetComponentSet<int>();
	std::string resultType = result->ComponentName();
	BOOST_TEST(ComponentManager::componentKeys["int"] == 0);
	BOOST_TEST(resultType._Equal("int"));
	BOOST_TEST(result != nullptr);
}

BOOST_AUTO_TEST_CASE(TestComponentManager_GetComponentKey) {
	ComponentManager::CreateComponentSet<int>();
	ComponentManager::CreateComponentSet<std::string>();

	BOOST_TEST(ComponentManager::GetComponentKey<int>() == 0);
	BOOST_TEST(ComponentManager::GetComponentKey<std::string>() == 1);
}

BOOST_AUTO_TEST_CASE(TestBitTracker_Has) {
	// ...0000 0000 0000 1100
	BitTracker tracker = BitTracker(0b1000);
	int index = 3;
	int index2 = 2;
	bool result = tracker.Has(index);
	BOOST_TEST(result == true);
	BOOST_TEST(tracker.Has(index2) == false);
}

BOOST_AUTO_TEST_CASE(TestBitTracker_Add) {
	BitTracker tracker = BitTracker();
	int index1 = 1;
	tracker.Add(index1);
	
	BOOST_TEST(tracker.Has(index1) == true);
	BOOST_TEST(tracker.bits == 0b0010);
}

BOOST_AUTO_TEST_CASE(TestBitTracker_Remove) {
	BitTracker tracker = BitTracker(0b0100);
	tracker.Remove(2);
	BOOST_TEST(tracker.Has(2) == false);
	BOOST_TEST(tracker.bits == 0b0000);
}

BOOST_AUTO_TEST_CASE(TestEntityManager_CreateEntity) {
	Entity testEntity = EntityManager::CreateEntity();
	
	BOOST_TEST(EntityManager::activeEntityBits[testEntity._id].bits == 0);
	BOOST_TEST(EntityManager::entityCount == 1);
	BOOST_TEST(testEntity._id == 0);
}

BOOST_AUTO_TEST_CASE(TestEntityManager_AddComponentKey) {
	Entity testEntity = EntityManager::CreateEntity();
	EntityManager::AddComponentKey(&testEntity, 1);
	BOOST_TEST(EntityManager::activeEntityBits[testEntity._id].bits == 0b0010);
}

BOOST_AUTO_TEST_CASE(TestEntityManager_RemoveComponentKey) {
	Entity testEntity = EntityManager::CreateEntity();
	EntityManager::AddComponentKey(&testEntity, 1);
	EntityManager::RemoveComponentKey(&testEntity, 1);
	BOOST_TEST(EntityManager::activeEntityBits[testEntity._id].bits == 0b0000);
}

BOOST_AUTO_TEST_CASE(TestEntityManager_DestroyEntity) {
	Entity testEntity = EntityManager::CreateEntity();
	EntityManager::DestroyEntity(&testEntity);
	BOOST_TEST(EntityManager::deadEntities.Count == 1);
	
	BOOST_TEST(EntityManager::activeEntityBits[testEntity._id].bits == 0);
}

BOOST_AUTO_TEST_CASE(TestEntityManager_DestroyEntityWithComponents) {
	Entity entity = EntityManager::CreateEntity();
	ComponentManager::CreateComponentSet<int>()->AddEntity(entity._id);
	EntityManager::DestroyEntity(&entity);
	BOOST_TEST(EntityManager::deadEntities.Count == 1);
	BOOST_TEST(EntityManager::activeEntityBits[entity._id].bits == 0);
}

BOOST_AUTO_TEST_CASE(TestEntity_HasComponent)
{
	Entity entity(0);
	ComponentManager::CreateComponentSet<int>();

	entity.AddComponent<int>();

	BOOST_TEST(entity.HasComponent<int>() == true);
}

BOOST_AUTO_TEST_CASE(TestEntity_HasNoComponent)
{
	Entity entity(0);

	BOOST_TEST(entity.HasComponent<int>() == false);
}

BOOST_AUTO_TEST_CASE(TestEntity_AddComponent)
{
	Entity entity(0);
	ComponentManager::CreateComponentSet<int>();

	entity.AddComponent<int>();

	BOOST_TEST(entity.HasComponent<int>() == true);
	BOOST_TEST(ComponentManager::GetComponentSet<int>()->HasEntity(entity._id) == true);
}

BOOST_AUTO_TEST_CASE(TestEntity_RemoveComponent)
{
	Entity entity(0);
	ComponentManager::CreateComponentSet<int>();

	entity.AddComponent<int>();
	entity.RemoveComponent<int>();

	BOOST_TEST(entity.HasComponent<int>() == false);
	BOOST_TEST(ComponentManager::GetComponentSet<int>()->HasEntity(entity._id) == false);
}

BOOST_AUTO_TEST_CASE(TestEntity_GetComponent)
{
	Entity entity(0);
	ComponentManager::CreateComponentSet<int>();

	entity.AddComponent<int>();

	int* component = entity.GetComponent<int>();

	BOOST_TEST(component != nullptr);
}

BOOST_AUTO_TEST_CASE(TestEntity_GetNoComponent)
{
	Entity entity(0);

	int* component = entity.GetComponent<int>();

	BOOST_TEST(component == nullptr);
}

BOOST_AUTO_TEST_CASE(TestEntity_Destroy)
{
	Entity entity(0);
	ComponentManager::CreateComponentSet<int>();

	entity.AddComponent<int>();
	entity.Destroy();

	BOOST_TEST(EntityManager::deadEntities.Count == 1);
	BOOST_TEST(EntityManager::activeEntityBits[entity._id].bits == 0);
}

BOOST_AUTO_TEST_CASE(TestEntity_Equals)
{
	Entity entity1(0);
	Entity entity2(0);
	Entity entity3(42);

	BOOST_TEST(entity1.Equals(&entity2) == true);
	BOOST_TEST(entity1.Equals(&entity3) == false);
}


BOOST_AUTO_TEST_SUITE_END()
