#include "Hurricane.h"

namespace test_absyn { 

using namespace swift::absyn;

Hurricane::Hurricane() :root(NULL){}

Hurricane::~Hurricane() {
  if (root != NULL) delete root;
}

BlogProgram* Hurricane::getRoot() {
  return root;
}

void Hurricane::build(){
  BlogProgram *blog = new BlogProgram(0, 0);
  root = blog;
  /*
  type City;
  type PrepLevel;
  type DamageLevel;
  */
  {
    blog->add(new TypDecl(0, 0, Symbol("City")));
    blog->add(new TypDecl(0, 0, Symbol("PrepLevel")));
    blog->add(new TypDecl(0, 0, Symbol("DamageLevel")));
  }
  /*
  distinct City A, B;
  distinct PrepLevel Low, High;
  distinct DamageLevel Severe, Mild;
  */
  {
    DistinctDecl*dis;
    dis = new DistinctDecl(0, 0, Symbol("City"));
    dis->add(Symbol("A"), 1);
    dis->add(Symbol("B"), 1);
    blog->add(dis);
    dis = new DistinctDecl(0, 0, Symbol("PrepLevel"));
    dis->add(Symbol("Low"), 1);
    dis->add(Symbol("High"), 1);
    blog->add(dis);
    dis = new DistinctDecl(0, 0, Symbol("DamageLevel"));
    dis->add(Symbol("Severe"), 1);
    dis->add(Symbol("Mild"), 1);
    blog->add(dis);
  }
  /*
  random City First  ~ UniformChoice({c for City c});
  */
  {
    SetExpr *se = new TupleSetExpr(0, 0, 
      std::vector<Expr*>({ new FuncApp(0, 0, Symbol("c")) }),
      std::vector<VarDecl>({ VarDecl(0, 0, Symbol("City"), Symbol("c")) }),NULL);
    FuncApp *dis = new FuncApp(0, 0, Symbol("UniformChoice"));
    dis->add(se);
    FuncDecl *fd = new FuncDecl(0, 0, true, Symbol("City"), Symbol("First"), dis);
    blog->add(fd);
  }
  /*
  random PrepLevel Prep(City c) ~
    if (First == c) then Categorical({High -> 0.5, Low -> 0.5})
    else 
      case Damage(First) in
        {Severe ->  ~ Categorical({High -> 0.9, Low -> 0.1}),
         Mild ->  ~ Categorical({High -> 0.1, Low -> 0.9})};
  };
  */
  {
    Expr *cond, *thn, *els;
    { // Cond
      OpExpr *eq = new OpExpr(0, 0, AbsynConstant::EQ,
        new FuncApp(0, 0, Symbol("First")), new FuncApp(0, 0, Symbol("c")));

      cond = eq;
    }
    { // Then Clause
      MapExpr *map = new MapExpr(0, 0);
      map->addMap(new FuncApp(0, 0, Symbol("High")), new DoubleLiteral(0, 0, 0.5));
      map->addMap(new FuncApp(0, 0, Symbol("Low")), new DoubleLiteral(0, 0, 0.5));
      FuncApp *cat = new FuncApp(0, 0, Symbol("Categorical"));
      cat->add(map);

      thn = cat;
    }
    { // Else Clause
      FuncApp *dam_f = new FuncApp(0, 0, Symbol("Damage"));
      dam_f->add(new FuncApp(0, 0, Symbol("First")));
      MapExpr *map_s = new MapExpr(0, 0);
      map_s->addMap(new FuncApp(0, 0, Symbol("High")), new DoubleLiteral(0, 0, 0.9));
      map_s->addMap(new FuncApp(0, 0, Symbol("Low")), new DoubleLiteral(0, 0, 0.1));
      FuncApp *cat_s = new FuncApp(0, 0, Symbol("Categorical"));
      cat_s->add(map_s);
      MapExpr *map_m = new MapExpr(0, 0);
      map_m->addMap(new FuncApp(0, 0, Symbol("High")), new DoubleLiteral(0, 0, 0.1));
      map_m->addMap(new FuncApp(0, 0, Symbol("Low")), new DoubleLiteral(0, 0, 0.9));
      FuncApp *cat_m = new FuncApp(0, 0, Symbol("Categorical"));
      cat_m->add(map_m);
      MapExpr *map_t = new MapExpr(0, 0);
      map_t->addMap(new FuncApp(0, 0, Symbol("Severe")), cat_s);
      map_t->addMap(new FuncApp(0, 0, Symbol("Mild")), cat_m);
      CaseExpr *cas_els = new CaseExpr(0, 0, dam_f, map_t);

      els = cas_els;
    }
    IfExpr *ife = new IfExpr(0, 0, cond, thn, els);
    FuncDecl *func = new FuncDecl(0, 0, true, Symbol("PrepLevel"), Symbol("Prep"), ife);
    func->addArg(VarDecl(0, 0, Symbol("City"), Symbol("c")));

    blog->add(func);
  }
  /*
  random DamageLevel Damage(City c) ~
    case Prep(c) in 
    {High ->  ~ Categorical({Severe -> 0.2, Mild -> 0.8}),
     Low ->  ~ Categorical({Severe -> 0.8, Mild -> 0.2})};
  };
  */
  {
    FuncApp *cat_h = new FuncApp(0, 0, Symbol("Categorical"));
    MapExpr *map_h = new MapExpr(0, 0);
    map_h->addMap(new FuncApp(0, 0, Symbol("Severe")), new DoubleLiteral(0, 0, 0.2));
    map_h->addMap(new FuncApp(0, 0, Symbol("Mild")), new DoubleLiteral(0, 0, 0.8));
    cat_h->add(map_h);
    FuncApp *cat_l = new FuncApp(0, 0, Symbol("Categorical"));
    MapExpr *map_l = new MapExpr(0, 0);
    map_l->addMap(new FuncApp(0, 0, Symbol("Severe")), new DoubleLiteral(0, 0, 0.8));
    map_l->addMap(new FuncApp(0, 0, Symbol("Mild")), new DoubleLiteral(0, 0, 0.2));
    cat_l->add(map_l);
    FuncApp *prep = new FuncApp(0, 0, Symbol("Prep"));
    prep->add(new FuncApp(0, 0, Symbol("c")));
    MapExpr *map_t = new MapExpr(0, 0);
    map_t->addMap(new FuncApp(0, 0, Symbol("High")), cat_h);
    map_t->addMap(new FuncApp(0, 0, Symbol("Low")), cat_l);
    CaseExpr *cas = new CaseExpr(0,0,prep,map_t);

    FuncDecl* func = new FuncDecl(0, 0, true, Symbol("DamageLevel"), Symbol("Damage"), cas);
    func->addArg(VarDecl(0, 0, Symbol("City"), Symbol("c")));
    blog->add(func);
  }
  /*
  obs Damage(First) = Severe;
  */
  {
    FuncApp *dam = new FuncApp(0, 0, Symbol("Damage"));
    dam->add(new FuncApp(0, 0, Symbol("First")));
    Evidence *obs = new Evidence(0, 0, dam, new FuncApp(0, 0, Symbol("Severe")));
    blog->add(obs);
  }
  /*
  query First;
  query Damage(A);
  query Damage(B);
  */
  {
    blog->add(new Query(0, 0, new FuncApp(0, 0, Symbol("First"))));
    FuncApp *dam_A = new FuncApp(0, 0, Symbol("Damage"));
    dam_A->add(new FuncApp(0, 0, Symbol("A")));
    blog->add(new Query(0, 0, dam_A));
    FuncApp *dam_B = new FuncApp(0, 0, Symbol("Damage"));
    dam_B->add(new FuncApp(0, 0, Symbol("B")));
    blog->add(new Query(0, 0, dam_B));
  }
}

void Hurricane::test(FILE *file)
{
  build();
  if(file != NULL) root->print(file, 0);
}

}
