#include "stdafx.h"
#include "CppUnitTest.h"

#define __P2ASSERT_H__

#include "../Motor2D_1er/p2SString.h"
#include "../Motor2D_1er/p2List.h"
#include "../Motor2D_1er/p2Tree.h"
#include "../Motor2D_1er/p2Vector2.h"
#include "../Motor2D_1er/p2Point.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void TLOG(const char* str, ...)
{
	static char tmp[4096];
	static va_list  ap;

	va_start( ap, str );
	vsprintf_s( tmp, 4096, str, ap );
	va_end( ap );

	strcat_s(tmp, 4096, "\n");
	Logger::WriteMessage(tmp);
}

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		// SWAP ----------------------------------------------
		TEST_METHOD(swap_int)
		{
			int a = 5;
			int b = 8;
			SWAP(a, b);
			Assert::AreEqual(a, 8);
			Assert::AreEqual(b, 5);
		}

		TEST_METHOD(swap_SString)
		{
			p2SString a("hola");
			p2SString b("mundo");
			SWAP(a, b);
			Assert::IsTrue(a == "mundo");
			Assert::IsTrue(b == "hola");
		}

		TEST_METHOD(swap_p2Vector2)
		{
			p2Vector2 a;
			p2Vector2 b;
			a.x = a.y = 1.0f;
			b = a;
			b.x = b.y = 2.0f;
			SWAP(a, b);
			Assert::IsTrue(a.x == 2.0f);
			Assert::IsTrue(b.x == 1.0f);
		}

		TEST_METHOD(Point2D_int)
		{
			p2Point<int> a;
			p2Point<int> b;

			a.x = a.y = 5;
			b = a;

			Assert::AreEqual(b.x, 5);
			Assert::AreEqual(b.y, 5);
		}

		TEST_METHOD(Point2D_double)
		{
			p2Point<double> a;
			p2Point<double> b;

			a.x = a.y = 3.1415;
			b = a;
			
			Assert::AreEqual(b.x, 3.1415);
			Assert::AreEqual(b.y, 3.1415);

			p2Point<p2SString> juas;
			juas.x = "hola";
			juas.y = "mundo";
			juas.DistanceTo(juas);
		}

		// p2SString -----------------------------------------
		TEST_METHOD(SString_empty_ctor)
		{
			p2SString s;
			Assert::AreEqual((int) s.Length(), 0);
		}
		TEST_METHOD(SString_char_ctor)
		{
			p2SString s("Hola mundo");
			Assert::AreEqual((int)s.Length(), 10);
		}
		TEST_METHOD(SString_format_ctor)
		{
			p2SString s("%s %s %d", "hola", "mundo", 12);
			Assert::AreEqual((int)s.Length(), 13);
		}
		TEST_METHOD(SString_copy_ctor)
		{
			p2SString s("%s %s %d", "hola", "mundo", 12);
			p2SString s2(s);
			Assert::AreEqual((int)s2.Length(), 13);
		}
		TEST_METHOD(SString_op_equal)
		{
			p2SString s1("Hola mundo");
			p2SString s2("Hola mundo");
			Assert::IsTrue(s1 == s2);
		}
		TEST_METHOD(SString_op_notequal)
		{
			p2SString s1("Hola mundo 1");
			p2SString s2("Hola mundo 2");
			Assert::IsTrue(s1 != s2);
		}
		TEST_METHOD(SString_op_equal_cstr)
		{
			p2SString s1("Hola mundo");
			Assert::IsTrue(s1 == "Hola mundo");
		}
		TEST_METHOD(SString_op_notequal_cstr)
		{
			p2SString s1("Hola mundo 1");
			Assert::IsTrue(s1 != "test");
		}
		TEST_METHOD(SString_op_assign)
		{
			p2SString s1("Hola mundo");
			p2SString s2;
			s2 = s1;
			Assert::IsTrue(s2 == "Hola mundo");
		}
		TEST_METHOD(SString_op_assign_cstr)
		{
			p2SString s1("Hola mundo");
			s1 = "this is a test";
			Assert::IsTrue(s1 == "this is a test");
		}
		TEST_METHOD(SString_op_concat)
		{
			p2SString s1("Hola mundo");
			p2SString s2("this is a test");
			s1 += s2;
			Assert::IsTrue(s1 == "Hola mundothis is a test");
		}
		TEST_METHOD(SString_op_concat_cstr)
		{
			p2SString s1("Hola mundo");
			s1 += "this is a test";
			Assert::IsTrue(s1 == "Hola mundothis is a test");
		}
		TEST_METHOD(SString_length)
		{
			p2SString s1("Hola mundo");
			Assert::AreEqual((int)s1.Length(), 10);
		}
		TEST_METHOD(SString_capacity)
		{
			p2SString s1("Hola mundo");
			Assert::AreEqual((int)s1.GetCapacity(), 11);
		}
		TEST_METHOD(SString_getstr)
		{
			p2SString s1("Hola mundo");
			Assert::AreEqual((int)strcmp("Hola mundo", s1.GetString()), 0);
		}
		TEST_METHOD(SString_clear)
		{
			p2SString s1("Hola mundo");
			s1.Clear();
			Assert::AreEqual((int)s1.Length(), 0);
		}

		// p2List --------------------------------------------
		TEST_METHOD(ListAdd)
		{
			p2List<int> mylist;
			Assert::AreEqual((int)mylist.add(5), 1);
		}

		TEST_METHOD(ListDel)
		{
			p2List<int> mylist;
			mylist.add(5);
			Assert::IsTrue(mylist.del(mylist.start));
			Assert::AreEqual((int)mylist.count(), 0);
		}

		TEST_METHOD(ListClear)
		{
			p2List<int> mylist;
			mylist.add(1); mylist.add(2); mylist.add(3);
			mylist.clear();
			Assert::AreEqual((int)mylist.count(), 0);
		}

		TEST_METHOD(ListBrackets)
		{
			p2List<int> mylist;
			mylist.add(5);
			Assert::AreEqual(mylist[0], 5);
		}

		// p2Tree --------------------------------------------
		TEST_METHOD(TreeAdd)
		{
			p2Tree<int> tree(0);
			
			tree.Add(100, 0);
			tree.Add(200, 0);
			tree.Add(300, 0);
			
			tree.Add(101, 100);
			tree.Add(102, 100);
			tree.Add(103, 100);
			
			tree.Add(210, 200);
			tree.Add(220, 200);
	
			tree.Add(221, 220);

			// Just do not test that we run forever because of the recursion
			Assert::AreEqual(1,1);
		}

		TEST_METHOD(TreeGather)
		{
			p2Tree<int> tree(0);
			
			tree.Add(100, 0);
			tree.Add(200, 0);
			tree.Add(300, 0);
			tree.Add(101, 100);
			tree.Add(102, 100);
			tree.Add(103, 100);
			tree.Add(210, 200);
			tree.Add(220, 200);
			tree.Add(221, 220);
			
			p2List<p2TreeNode<int>*> list;
			tree.trunk.GatherAll(&list);
			
			Assert::AreEqual((int)list.count(), 10);

			Assert::AreEqual((int)list[0]->data, 0);
			Assert::AreEqual((int)list[1]->data, 100);
			Assert::AreEqual((int)list[2]->data, 101);
			Assert::AreEqual((int)list[3]->data, 102);
			Assert::AreEqual((int)list[4]->data, 103);
			Assert::AreEqual((int)list[5]->data, 200);
			Assert::AreEqual((int)list[6]->data, 210);
			Assert::AreEqual((int)list[7]->data, 220);
			Assert::AreEqual((int)list[8]->data, 221);
			Assert::AreEqual((int)list[9]->data, 300);

		}

		TEST_METHOD(TreeGatherData)
		{
			p2Tree<int> tree(0);
			
			tree.Add(100, 0);
			tree.Add(200, 0);
			tree.Add(300, 0);
			tree.Add(101, 100);
			tree.Add(102, 100);
			tree.Add(103, 100);
			tree.Add(210, 200);
			tree.Add(220, 200);
			tree.Add(221, 220);
			
			p2List<int> list;
			tree.trunk.GatherAllData(&list);
			
			Assert::AreEqual((int)list.count(), 10);

			Assert::AreEqual((int)list[0], 0);
			Assert::AreEqual((int)list[1], 100);
			Assert::AreEqual((int)list[2], 101);
			Assert::AreEqual((int)list[3], 102);
			Assert::AreEqual((int)list[4], 103);
			Assert::AreEqual((int)list[5], 200);
			Assert::AreEqual((int)list[6], 210);
			Assert::AreEqual((int)list[7], 220);
			Assert::AreEqual((int)list[8], 221);
			Assert::AreEqual((int)list[9], 300);

		}

		TEST_METHOD(TreeDelRecursive)
		{
			p2Tree<int> tree(0);
			
			tree.Add(100, 0);
			tree.Add(200, 0);
			tree.Add(300, 0);
			tree.Add(101, 100);
			tree.Add(102, 100);
			tree.Add(103, 100);
			tree.Add(210, 200);
			tree.Add(220, 200);
			tree.Add(221, 220);
			
			tree.DelRecursive(200);

			p2List<p2TreeNode<int>*> list;
			tree.trunk.GatherAll(&list);
			
			Assert::AreEqual((int)list.count(), 6);

			Assert::AreEqual((int)list[0]->data, 0);
			Assert::AreEqual((int)list[1]->data, 100);
			Assert::AreEqual((int)list[2]->data, 101);
			Assert::AreEqual((int)list[3]->data, 102);
			Assert::AreEqual((int)list[4]->data, 103);
			Assert::AreEqual((int)list[5]->data, 300);

			for(p2List_item<p2TreeNode<int>*>* item =  list.start; item != NULL; item = item->next)
			{
				p2TreeNode<int>* child = item->data;
				TLOG("node: %d parent: %d", child->data, (child->parent) ? child->parent->data : -1);
			}

		}

		TEST_METHOD(TreeClear)
		{
			p2Tree<int> tree(0);
			
			tree.Add(100, 0);
			tree.Add(200, 0);
			tree.Add(300, 0);
			tree.Add(101, 100);
			tree.Add(102, 100);
			tree.Add(103, 100);
			tree.Add(210, 200);
			tree.Add(220, 200);
			tree.Add(221, 220);
			
			tree.Clear();

			p2List<p2TreeNode<int>*> list;
			tree.trunk.GatherAll(&list);
			
			Assert::AreEqual((int)list.count(), 1);
		}


		TEST_METHOD(TreeFind)
		{
			p2Tree<int> tree(0);
			
			tree.Add(100, 0);
			tree.Add(200, 0);
			tree.Add(300, 0);
			tree.Add(101, 100);
			tree.Add(102, 100);
			tree.Add(103, 100);
			tree.Add(210, 200);
			tree.Add(220, 200);
			tree.Add(221, 220);
			
			p2TreeNode<int>* p = tree.trunk.FindRecursive(210);
			p2TreeNode<int>* p2 = tree.trunk.FindRecursive(999);

			Assert::AreEqual((int)p->data, 210);
			Assert::IsNull(p2);
		}

		TEST_METHOD(TreeRelocate)
		{
			p2Tree<int> tree(0);
			
			tree.Add(100, 0);
			tree.Add(200, 0);
			tree.Add(300, 0);
			tree.Add(101, 100);
			tree.Add(102, 100);
			tree.Add(103, 100);
			tree.Add(210, 200);
			tree.Add(220, 200);
			tree.Add(221, 220);
			
			p2TreeNode<int>* p = tree.trunk.FindRecursive(200);
			Assert::AreEqual((int)p->parent->data, 0);
			
			Assert::IsTrue(tree.Relocate(200, 103));

			p = tree.trunk.FindRecursive(200);
			Assert::AreEqual((int)p->parent->data, 103);

			p2List<p2TreeNode<int>*> list;
			tree.trunk.GatherAll(&list);
			for(p2List_item<p2TreeNode<int>*>* item =  list.start; item != NULL; item = item->next)
			{
				p2TreeNode<int>* child = item->data;
				TLOG("node: %d parent: %d", child->data, (child->parent) ? child->parent->data : -1);
			}

		}
	};
}