//#define BOOST_TEST_MODULE ECSTest
//
//#include <boost/test/included/unit_test.hpp>
//
//#include <boost/test/unit_test.hpp>
//#include "PackedArray.h"
//#include "ComponentSet.h"
//#include "ComponentManager.h"
//#include "EntityManager.h" 
//#include <bitset>
//
//struct ECSTestCases {
//	ECSTestCases() { }
//	~ECSTestCases() {}
//};
//
//BOOST_GLOBAL_FIXTURE(ECSTestCases);
//BOOST_AUTO_TEST_SUITE(ECSTest)
//
//BOOST_AUTO_TEST_CASE(TestPackedArray_AddItem) {
//	std::cout << "TestPackedArray_AddItem\n";
//	PackedArray<uint8_t> packedArray;
//	uint8_t item = 42;
//	packedArray.Add(item);
//
//	// Verify that the item is added and can be retrieved
//	uint8_t* retrievedItem = packedArray.Get(0);
//	BOOST_TEST(*retrievedItem == item);
//	BOOST_TEST(packedArray.Count == 1);
//}
//
//BOOST_AUTO_TEST_CASE(TestPackedArray_Resize) {
//	PackedArray<uint8_t> packedArray;
//	for (size_t i = 0; i < 12; i++)
//	{
//		packedArray.Add(i);
//	}
//	BOOST_TEST(*packedArray.Get(11) == 11);
//	BOOST_TEST(packedArray.Count == 12);
//	BOOST_TEST(packedArray.size == 16);
//}
//
//BOOST_AUTO_TEST_CASE(TestPackedArray_RemoveItem) {
//	std::cout << "TestPackedArray_RemoveItem\n";
//	PackedArray<uint8_t> packedArray;
//	uint8_t item1 = 42;
//	uint8_t item2 = 100;
//	packedArray.Add(item1);
//	packedArray.Add(item2);
//
//	// Remove an item from the packed array
//	packedArray.Remove(0);
//
//	// Verify that the item is removed and the count is updated
//	BOOST_TEST(packedArray.Count == 1);
//	BOOST_TEST(*packedArray.Get(0) == item2);
//}
//
//BOOST_AUTO_TEST_CASE(TestPackedArray_GetEmpty) {
//	std::cout << "TestPackedArray_GetEmpty\n";
//	PackedArray<uint8_t> packedArray;
//
//	uint8_t* retreivedItem = packedArray.Get(0);
//	// Verify that the retreived item results in a nullptr
//	BOOST_TEST(retreivedItem == nullptr);
//}
//BOOST_AUTO_TEST_CASE(TestPackedArray_Set) {
//	PackedArray<uint8_t> packedArray;
//	uint8_t a = 5;
//	packedArray.Add(1);
//	packedArray.Set(0, &a);
//
//	BOOST_TEST(*packedArray.Get(0) == 5);
//}
//BOOST_AUTO_TEST_CASE(TestComponentSet_AddEntity)
//{
//	std::cout << "TestComponentSet_AddEntity\n";
//	unsigned short entityId = 0;
//	unsigned short entityId2 = 42;
//	ComponentSet<uint8_t> intComponents(0);
//
//	intComponents.AddEntity(entityId);
//	intComponents.AddEntity(entityId2);
//	BOOST_TEST(intComponents.count == 2);
//	BOOST_TEST(intComponents.HasEntity(entityId));
//	BOOST_TEST(intComponents.HasEntity(entityId2));
//}
//
//BOOST_AUTO_TEST_CASE(TestComponentSet_GetComponent)
//{
//	std::cout << "TestComponentSet_GetComponent\n";
//	unsigned short entityId = 42;
//	ComponentSet<uint8_t> intComponents(0);
//
//	intComponents.AddEntity(entityId);
//	uint8_t* result = intComponents.GetComponent(entityId);
//
//	BOOST_TEST(result != nullptr);
//}
//
//BOOST_AUTO_TEST_CASE(TestComponentSet_SetEntityData)
//{
//	std::cout << "TestComponentSet_GetComponent\n";
//	unsigned short entityId = 42;
//	ComponentSet<uint8_t> intComponents(0);
//
//	intComponents.AddEntity(entityId);
//	uint8_t a = 10;
//	intComponents.SetEntityData(entityId, &a);
//	uint8_t* result = intComponents.GetComponent(entityId);
//	*result = *result + 1;
//
//	BOOST_TEST(result != nullptr);
//	BOOST_TEST(*result = a);
//	BOOST_TEST(*result = 11);
//}
//
//BOOST_AUTO_TEST_CASE(TestComponentSet_GetNoComponent)
//{
//	std::cout << "TestComponentSet_GetNoComponent\n";
//	unsigned short entityId = 42;
//	ComponentSet<uint8_t> intComponents(0);
//
//	uint8_t* result = intComponents.GetComponent(entityId);
//
//	BOOST_TEST(result == nullptr);
//}
//
//BOOST_AUTO_TEST_CASE(TestComponentSet_HasEntity)
//{
//	std::cout << "TestComponentSet_HasEntity\n";
//	unsigned short entityId = 0;
//	unsigned short entityId2 = 42;
//	ComponentSet<uint8_t> intComponents(0);
//
//	intComponents.AddEntity(entityId);
//	BOOST_TEST(intComponents.HasEntity(entityId) == TRUE);
//	BOOST_TEST(intComponents.HasEntity(entityId2) == FALSE);
//}
//
//BOOST_AUTO_TEST_CASE(TestComponentSet_RemoveOneEntity)
//{
//	std::cout << "TestComponentSet_RemoveOneEntity\n";
//	unsigned short entityId = 0;
//	ComponentSet<uint8_t> intComponents(0);
//
//	intComponents.AddEntity(entityId);
//
//	intComponents.RemoveEntity(entityId);
//
//	BOOST_TEST(intComponents.count == 0);
//	BOOST_TEST(intComponents.HasEntity(entityId) == FALSE);
//}
//
//BOOST_AUTO_TEST_CASE(TestComponentSet_RemoveEntity)
//{
//	std::cout << "TestComponentSet_RemoveEntity\n";
//	unsigned short entityId = 0;
//	unsigned short entityId2 = 42;
//	ComponentSet<uint8_t> intComponents(0);
//
//	//intComponents.AddEntity(entityId);
//	intComponents.AddEntity(entityId2);
//
//	intComponents.RemoveEntity(entityId);
//
//	BOOST_TEST(intComponents.count == 1);
//	BOOST_TEST(intComponents.HasEntity(entityId) == FALSE);
//	BOOST_TEST(intComponents.HasEntity(entityId2) == TRUE);
//}
//
//BOOST_AUTO_TEST_CASE(TestComponentManager_CreateComponentSet) {
//	std::cout << "TestComponentManager_CreateComponentSet\n";
//	ComponentSet<uint8_t>* result = ComponentManager::CreateComponentSet<uint8_t>();
//
//	std::string resultType = result->ComponentName();
//	delete result;
//	BOOST_TEST(resultType._Equal("uint8_t"));
//	BOOST_TEST(result != nullptr);
//	BOOST_TEST(ComponentManager::GetComponentKey<uint8_t>() == 0);
//	
//}
//
//BOOST_AUTO_TEST_CASE(TestComponentManager_GetComponentSet) {
//	std::cout << "TestComponentManager_GetComponentSet\n";
//	auto* c1 = ComponentManager::CreateComponentSet<uint8_t>();
//
//	ComponentSet<uint8_t>* result = ComponentManager::GetComponentSet<uint8_t>();
//	std::string resultType = result->ComponentName();
//	BOOST_TEST(ComponentManager::componentKeys["uint8_t"] == 0);
//	BOOST_TEST(resultType._Equal("uint8_t"));
//	BOOST_TEST(result != nullptr);
//	delete c1;
//}
//
//BOOST_AUTO_TEST_CASE(TestComponentManager_GetComponentKey) {
//	std::cout << "TestComponentManager_GetComponentKey\n";
//	auto *c1 = ComponentManager::CreateComponentSet<uint8_t>();
//	auto* c2 = ComponentManager::CreateComponentSet<std::string>();
//
//	BOOST_TEST(ComponentManager::GetComponentKey<uint8_t>() == 0);
//	BOOST_TEST(ComponentManager::GetComponentKey<std::string>() == 1);
//	delete c1;
//	delete c2;
//}
//
//BOOST_AUTO_TEST_CASE(TestBitTracker_Has) {
//	std::cout << "TestBitTracker_Has\n";
//	// ...0000 0000 0000 1100
//	BitTracker tracker = BitTracker(0b1000);
//	uint8_t index = 3;
//	uint8_t index2 = 2;
//	bool result = tracker.Has(index);
//	BOOST_TEST(result == true);
//	BOOST_TEST(tracker.Has(index2) == false);
//}
//
//BOOST_AUTO_TEST_CASE(TestBitTracker_Add) {
//	std::cout << "TestBitTracker_Add\n";
//	BitTracker tracker = BitTracker();
//	uint8_t index1 = 1;
//	tracker.Add(index1);
//	
//	BOOST_TEST(tracker.Has(index1) == true);
//	BOOST_TEST(tracker.bits == 0b0010);
//}
//
//BOOST_AUTO_TEST_CASE(TestBitTracker_Remove) {
//	std::cout << "TestBitTracker_Remove\n";
//	BitTracker tracker = BitTracker(0b0100);
//	tracker.Remove(2);
//	BOOST_TEST(tracker.Has(2) == false);
//	BOOST_TEST(tracker.bits == 0b0000);
//}
//
//BOOST_AUTO_TEST_CASE(TestEntityManager_CreateEntity) {
//	std::cout << "TestEntityManager_CreateEntity\n";
//	Entity * testEntity = EntityManager::CreateEntity();
//	
//	BOOST_TEST(EntityManager::activeEntityBits[testEntity->_id].bits == 0);
//	BOOST_TEST(EntityManager::entityCount == 1);
//	BOOST_TEST(testEntity->_id == 0);
//}
//
//BOOST_AUTO_TEST_CASE(TestEntityManager_AddComponentKey) {
//	std::cout << "TestEntityManager_AddComponentKey\n";
//	Entity * testEntity = EntityManager::CreateEntity();
//	EntityManager::AddComponentKey(testEntity, 1);
//	BOOST_TEST(EntityManager::activeEntityBits[testEntity->_id].bits == 0b0010);
//}
//
//BOOST_AUTO_TEST_CASE(TestEntityManager_RemoveComponentKey) {
//	std::cout << "TestEntityManager_RemoveComponentKey\n";
//	Entity * testEntity = EntityManager::CreateEntity();
//	EntityManager::AddComponentKey(testEntity, 1);
//	EntityManager::RemoveComponentKey(testEntity, 1);
//	BOOST_TEST(EntityManager::activeEntityBits[testEntity->_id].bits == 0b0000);
//}
//
//BOOST_AUTO_TEST_CASE(TestEntityManager_DestroyEntity) {
//	std::cout << "TestEntityManager_DestroyEntity\n";
//	Entity * testEntity = EntityManager::CreateEntity();
//	EntityManager::DestroyEntity(testEntity);
//	BOOST_TEST(EntityManager::deadEntities.Count == 1);
//	
//	BOOST_TEST(EntityManager::activeEntityBits[testEntity->_id].bits == 0);
//}
//
//BOOST_AUTO_TEST_CASE(TestEntityManager_DestroyEntityWithComponents) {
//	std::cout << "TestEntityManager_DestroyEntityWithComponents\n";
//	Entity * entity = EntityManager::CreateEntity();
//	auto* c1 = ComponentManager::CreateComponentSet<uint8_t>();
//	c1->AddEntity(entity->_id);
//	EntityManager::DestroyEntity(entity);
//	BOOST_TEST(EntityManager::deadEntities.Count == 1);
//	BOOST_TEST(EntityManager::activeEntityBits[entity->_id].bits == 0);
//	delete c1;
//}
//
//BOOST_AUTO_TEST_CASE(TestEntity_HasComponent)
//{
//	std::cout << "TestEntity_HasComponent\n";
//	Entity entity(0);
//	auto c1 = ComponentManager::CreateComponentSet<uint8_t>();
//	BOOST_TEST(entity.HasComponent<uint8_t>() == false);
//	entity.AddComponent<uint8_t>();
//
//	BOOST_TEST(entity.HasComponent<uint8_t>() == true);
//	delete c1;
//}
//
//BOOST_AUTO_TEST_CASE(TestEntity_HasNoComponent)
//{
//	std::cout << "TestEntity_HasNoComponent\n";
//	Entity entity(0);
//
//	BOOST_TEST(entity.HasComponent<uint8_t>() == false);
//}
//
//BOOST_AUTO_TEST_CASE(TestEntity_AddComponent)
//{
//	std::cout << "TestEntity_AddComponent\n";
//	Entity * entity = EntityManager::CreateEntity();
//	auto c1 = ComponentManager::CreateComponentSet<uint8_t>();
//
//	entity->AddComponent<uint8_t>();
//	
//	BOOST_TEST(entity->HasComponent<uint8_t>() == true);
//	BOOST_TEST(ComponentManager::GetComponentSet<uint8_t>()->HasEntity(entity->_id) == true);
//	delete c1;
//}
//BOOST_AUTO_TEST_CASE(TestEntity_EditAddComponent) {
//	std::cout << "TestEntity_EditAddComponent\n";
//	Entity * entity = EntityManager::CreateEntity();
//	struct INT { uint8_t value; };
//	auto c1 = ComponentManager::CreateComponentSet<INT>();
//
//	entity->AddComponent<INT>()->value = 5;
//
//	BOOST_TEST(entity->HasComponent<INT>() == true);
//	BOOST_TEST(ComponentManager::GetComponentSet<INT>()->HasEntity(entity->_id) == true);
//	BOOST_TEST(entity->GetComponent<INT>()->value == 5);
//}
//BOOST_AUTO_TEST_CASE(TestEntity_RemoveComponent)
//{
//	std::cout << "TestEntity_RemoveComponent\n";
//	Entity * entity = EntityManager::CreateEntity();
//	auto c1 = ComponentManager::CreateComponentSet<uint8_t>();
//
//	entity->AddComponent<uint8_t>();
//	entity->RemoveComponent<uint8_t>();
//
//	BOOST_TEST(entity->HasComponent<uint8_t>() == false);
//	BOOST_TEST(ComponentManager::GetComponentSet<uint8_t>()->HasEntity(entity->_id) == false);
//	delete c1;
//}
//
//BOOST_AUTO_TEST_CASE(TestEntity_GetComponent)
//{
//	std::cout << "TestEntity_GetComponent\n";
//	Entity * entity = EntityManager::CreateEntity();
//	auto c1 = ComponentManager::CreateComponentSet<uint8_t>();
//
//	entity->AddComponent<uint8_t>();
//
//	uint8_t* component = entity->GetComponent<uint8_t>();
//
//	BOOST_TEST(component != nullptr);
//	delete c1;
//}
//
//BOOST_AUTO_TEST_CASE(TestEntity_SetComponent)
//{
//	std::cout << "TestEntity_GetComponent\n";
//	Entity* entity = EntityManager::CreateEntity();
//	auto c1 = ComponentManager::CreateComponentSet<uint8_t>();
//
//	entity->AddComponent<uint8_t>();
//	uint8_t a = 10;
//	entity->SetComponent(&a);
//	uint8_t* component = entity->GetComponent<uint8_t>();
//	*component = *component + 1;
//
//	BOOST_TEST(component != nullptr);
//	BOOST_TEST(*component == 11);
//	delete c1;
//}
//
//BOOST_AUTO_TEST_CASE(TestEntity_GetNoComponent)
//{
//	std::cout << "TestEntity_GetComponent\n";
//	Entity* entity = EntityManager::CreateEntity();
//	auto c1 = ComponentManager::CreateComponentSet<uint8_t>();
//
//	uint8_t* component = entity->GetComponent<uint8_t>();
//
//	BOOST_TEST(component == nullptr);
//	delete c1;
//}
//
//BOOST_AUTO_TEST_CASE(TestEntity_Destroy)
//{
//	std::cout << "TestEntity_Destroy\n";
//	Entity *entity = EntityManager::CreateEntity();
//	auto c1 = ComponentManager::CreateComponentSet<uint8_t>();
//
//	entity->AddComponent<uint8_t>();
//	entity->Destroy();
//
//	BOOST_TEST(EntityManager::deadEntities.Count == 1);
//	BOOST_TEST(EntityManager::activeEntityBits[entity->_id].bits == 0);
//	delete c1;
//}
//
//BOOST_AUTO_TEST_CASE(TestEntity_Equals)
//{
//	std::cout << "TestEntity_Equals\n";
//	Entity *entity1 = EntityManager::CreateEntity();
//	Entity entity2 = Entity(0);
//	Entity entity3 = Entity(42);
//
//	BOOST_TEST(entity1->Equals(&entity2) == true);
//	BOOST_TEST(entity1->Equals(&entity3) == false);
//}
//
//
//BOOST_AUTO_TEST_SUITE_END()
