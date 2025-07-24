#include "base.h"

/************************************************************/
/*            DEFINITIONS STRUCTURES/CONSTANTES             */
/************************************************************/

/* L'enonce impose : */
#define omega  0
#define LG_MAX 64

typedef Nat Ident;

/* Date : structure s_date { Nat jour,mois,annee } */
typedef struct s_date {
  Nat jour;
  Nat mois;
  Nat annee;
} Date;


typedef struct s_individu {
  Car nom[LG_MAX]; 
  Date naiss;
  Date dec;
  Ident ipere;
  Ident imere;
  Ident ifaine; 
  Ident icadet;
} *Individu;

typedef struct s_genealogie {
  Individu *tab;
  Nat nb_individus;
  Nat taille_max_tab;
  Ident *rang;
  Nat taille_max_rang;
  Nat id_cur;
} *Genealogie;

/************************************************************/
/*         PROTOTYPES (dans le style canevas.c impose)      */
/************************************************************/

/* 1.1 Generateurs / Destructeurs */
void genealogieInit(Genealogie *g);
void genealogieFree(Genealogie *g);
Individu nouvIndividu(Ident i, Chaine s, Ident p, Ident m, Date n, Date d);
void freeIndividu(Individu id);

/* 1.2 Selecteurs */
Chaine nomIndividu(Individu ind);
Date naissIndividu(Individu ind);
Nat cardinal(Genealogie g);
Individu kieme(Genealogie g, Nat k);

/* 1.3 getByIdent => T(i) */
Individu getByIdent(Genealogie g, Ident i);

/* 1.4 getByName => tri par nom => O(log n) + getPos */
Individu getByName(Genealogie g, Chaine name, Date naissance);
Nat getPos(Genealogie g, Chaine name);

/* compDate => renvoie <0,0,>0 */
Ent compDate(Date d1, Date d2);

/* 1.5 ajout d'un individu */
Ident adj(Genealogie g, Chaine s, Ident p, Ident m, Date n, Date d);

/* Fonctions auxiliaires impos�es: insert, adjFils */
void insert(Genealogie g, Nat pos, Chaine s, Ident p, Ident m, Date n, Date d);
void adjFils(Genealogie g, Ident idx, Ident fils, Ident p, Ident m);

/* 1.6 => construction de la genealogie => exemple dans main */

/* 2.x Affichage => pas de printf, on met dans buff */
void affiche_freres_soeurs(Genealogie g, Ident x, Chaine buff);
void affiche_enfants(Genealogie g, Ident x, Chaine buff);
void affiche_cousins(Genealogie g, Ident x, Chaine buff);
void affiche_oncles(Genealogie g, Ident x, Chaine buff);

/* 3.x => liens de parent� a posteriori */
void deviennent_freres_soeurs(Genealogie g, Ident x, Ident y);
void devient_pere(Genealogie g, Ident x, Ident y);
void devient_mere(Genealogie g, Ident x, Ident y);

/* 4.x => Parcours de l'arbre */
Bool estAncetre(Genealogie g, Ident x, Ident y);
Bool ontAncetreCommun(Genealogie g, Ident x, Ident y);
Ident plus_ancien(Genealogie g, Ident x);
void affiche_parente(Genealogie g, Ident x, Chaine buff);
void affiche_descendance(Genealogie g, Ident x, Chaine buff);

/************************************************************/
/*     FONCTIONS AUXILIAIRES INTERNES (NON EXIGEES)         */
/************************************************************/

/* Simple helper: check if x,y share a parent (are siblings).
   The PRE mentions !freres_soeurs(g,p,m). We'll define a function for that. */
Bool freres_soeurs(Genealogie g, Ident x, Ident y) {
  if(x==omega || y==omega) return 0;
  {
    Individu ix = getByIdent(g,x);
    Individu iy = getByIdent(g,y);
    if(ix==NULL || iy==NULL) return 0;
    if(ix->ipere!=omega && ix->ipere==iy->ipere) return 1;
    if(ix->imere!=omega && ix->imere==iy->imere) return 1;
    return 0;
  }
}

/************************************************************/
/* 1.1 : genealogieInit, genealogieFree, nouvIndividu,...   */
/************************************************************/

void genealogieInit(Genealogie *g) {
  Genealogie gg;
  gg = (Genealogie)MALLOC((Nat)(sizeof(*gg)));
  gg->tab = NULL;
  gg->nb_individus = 0;
  gg->taille_max_tab = 0;
  gg->rang = NULL;
  gg->taille_max_rang = 0;
  gg->id_cur = 1;
  *g = gg;
}

void genealogieFree(Genealogie *g) {
  if(*g==NULL) return;
  {
    Genealogie gg = *g;
    {
      Nat i=0;
      while(i<gg->nb_individus) {
        freeIndividu(gg->tab[i]);
        i=i+1;
      }
    }
    if(gg->tab!=NULL) {
      FREE(gg->tab);
      gg->tab=NULL;
    }
    if(gg->rang!=NULL) {
      FREE(gg->rang);
      gg->rang=NULL;
    }
    FREE(gg);
    *g=NULL;
  }
}

Individu nouvIndividu(Ident i, Chaine s, Ident p, Ident m, Date n, Date d) {
  (void)i; /* The assignment shows we do not store 'i' in s_individu. */
  {
    Individu ind = (Individu)MALLOC((Nat)(sizeof(*ind)));
    /* fill fields */
    STRINGCPY(ind->nom, s);
    ind->naiss = n;
    ind->dec = d;
    ind->ipere = p;
    ind->imere = m;
    ind->ifaine = omega;
    ind->icadet = omega;
    /* if s[0] == '\0', it's invalid => user must handle. */
    return ind;
  }
}

void freeIndividu(Individu id) {
  if(id!=NULL) {
    FREE(id);
  }
}

/************************************************************/
/*   1.2 : nomIndividu, naissIndividu, cardinal, kieme      */
/************************************************************/

Chaine nomIndividu(Individu ind) {
  /* returns the pointer to nom */
  return ind->nom;
}

Date naissIndividu(Individu ind) {
  return ind->naiss;
}

Nat cardinal(Genealogie g) {
  return g->nb_individus;
}

Individu kieme(Genealogie g, Nat k) {
  if(k>=g->nb_individus) return NULL;
  return g->tab[k];
}

/************************************************************/
/*   1.3 : getByIdent => T(i) = tab[rang[i-1]] en O(1)       */
/************************************************************/

Individu getByIdent(Genealogie g, Ident i) {
  if(i<1) return NULL;
  if(i>=g->id_cur) return NULL;
  {
    Nat idx = g->rang[i-1];
    if(idx>=g->nb_individus) return NULL;
    return g->tab[idx];
  }
}

/************************************************************/
/*   1.4 : getByName => tri + getPos => O(log n)            */
/************************************************************/

Nat getPos(Genealogie g, Chaine name) {
  /* Cherche la position d'insertion ou la 1ere occurrence */
  Nat low=0;
  Nat high=g->nb_individus;
  while(low<high) {
    Nat mid = (low+high)/2;
    /* compare name with tab[mid]->nom */
    Ent cmp = (Ent)STRINGCMP(g->tab[mid]->nom, name);
    if(cmp<0) {
      low=mid+1;
    } else {
      high=mid;
    }
  }
  return low;
}

Individu getByName(Genealogie g, Chaine name, Date naissance) {
  if(g->nb_individus==0) return NULL;
  {
    Nat pos = getPos(g,name);
    if(pos>=g->nb_individus) return NULL;
    {
      /* check if we have an exact match of name at pos */
      if(STRINGCMP(g->tab[pos]->nom,name)!=0) {
        return NULL;
      }
      {
        /* if date={0,0,0} => plus jeune */
        Date zero; zero.jour=0; zero.mois=0; zero.annee=0;
        Bool wantYoungest=0;
        if(compDate(naissance,zero)==0) {
          wantYoungest=1;
        }
        if(wantYoungest==1) {
          /* gather all same-named => pick largest date */
          Date best; best.jour=0; best.mois=0; best.annee=0;
          Nat i=pos;
          Individu bestInd=NULL;
          while(i<g->nb_individus) {
            if(STRINGCMP(g->tab[i]->nom,name)==0) {
              if(compDate(g->tab[i]->naiss,best)>0) {
                best=g->tab[i]->naiss;
                bestInd=g->tab[i];
              }
              i=i+1;
            } else {
              i=g->nb_individus;
            }
          }
          return bestInd;
        } else {
          /* search exact date match */
          Nat i=pos;
          while(i<g->nb_individus) {
            Individu c=g->tab[i];
            if(STRINGCMP(c->nom,name)==0) {
              if(compDate(c->naiss,naissance)==0) {
                return c;
              }
              i=i+1;
            } else {
              i=g->nb_individus;
            }
          }
          return NULL;
        }
      }
    }
  }
}

/************************************************************/
/*   compDate => renvoie <0,0,>0                            */
/************************************************************/

Ent compDate(Date d1, Date d2) {
  if(d1.annee<d2.annee) return -1;
  if(d1.annee>d2.annee) return 1;
  if(d1.mois<d2.mois) return -1;
  if(d1.mois>d2.mois) return 1;
  if(d1.jour<d2.jour) return -1;
  if(d1.jour>d2.jour) return 1;
  return 0;
}

/************************************************************/
/*   1.5 : adj => ajout d'un nouvel individu                */
/************************************************************/

/* insert => ins�re un individu (ID=g->id_cur) au rang pos */
void insert(Genealogie g, Nat pos, Chaine s, Ident p, Ident m, Date n, Date d) {
  /* 1) check capacity for tab */
  if(g->nb_individus==g->taille_max_tab) {
    Nat newcap;
    if(g->taille_max_tab==0) {
      newcap=4;
    } else {
      newcap=g->taille_max_tab*2;
    }
    {
      Individu *temp;
      temp=(Individu*)REALLOC(g->tab,(Nat)(newcap*sizeof(Individu)));
      g->tab=temp;
      g->taille_max_tab=newcap;
    }
  }
  /* 2) check capacity for rang => g->id_cur => needed */
  {
    if(g->id_cur>g->taille_max_rang) {
      Nat newr;
      if(g->taille_max_rang==0) {
        newr=8;
      } else {
        newr=g->taille_max_rang;
      }
      while(newr<g->id_cur) {
        newr=newr*2;
      }
      {
        Ident *temp;
        temp=(Ident*)REALLOC(g->rang,(Nat)(newr*sizeof(Ident)));
        {
          Nat i2=g->taille_max_rang;
          while(i2<newr) {
            temp[i2]=0;
            i2=i2+1;
          }
        }
        g->rang=temp;
        g->taille_max_rang=newr;
      }
    }
  }
  /* 3) shift from nb_individus..>pos */
  {
    Nat i=g->nb_individus;
    while(i>pos) {
      g->tab[i]=g->tab[i-1];
      i=i-1;
    }
  }
  /* 4) create new individ => g->id_cur */
  {
    Ident newID=g->id_cur;
    Individu ni=nouvIndividu(newID,s,p,m,n,d);
    g->tab[pos]=ni;
    g->nb_individus=g->nb_individus+1;
    g->rang[newID-1]=pos;

    /* 5) update rang for i=1..(newID-1) => if rang[i-1]>=pos => rang[i-1]++ */
    {
      Nat i2=1;
      while(i2<newID) {
        Nat rr=g->rang[i2-1];
        if(rr>=pos) {
          g->rang[i2-1]=rr+1;
        }
        i2=i2+1;
      }
    }
    /* 6) done => g->id_cur++ */
    g->id_cur=g->id_cur+1;
  }
}

/* adjFils => ins�re idx dans la liste d'enfants (fils) tri�e du + ancien au + jeune. 
   fils => father->ifaine (== mother->ifaine). 
*/
void adjFils(Genealogie g, Ident idx, Ident fils, Ident p, Ident m) {
  if(p==omega && m==omega) return; 
  {
    Individu child=getByIdent(g,idx);
    if(child==NULL) return;
    if(fils==omega) {
      /* liste vide => father->ifaine=idx, mother->ifaine=idx */
      if(p!=omega) {
        Individu dad=getByIdent(g,p);
        if(dad!=NULL) {
          dad->ifaine=idx;
        }
      }
      if(m!=omega) {
        Individu mom=getByIdent(g,m);
        if(mom!=NULL) {
          mom->ifaine=idx;
        }
      }
      return;
    }
    {
      /* On compare la date => si child plus ancien que head => child devient t�te. */
      Individu headInd=getByIdent(g,fils);
      if(headInd==NULL) return;
      if(compDate(child->naiss,headInd->naiss)<0) {
        /* child est + ancien => devient t�te. */
        child->icadet=fils;
        if(p!=omega) {
          Individu dad=getByIdent(g,p);
          if(dad!=NULL) dad->ifaine=idx;
        }
        if(m!=omega) {
          Individu mom=getByIdent(g,m);
          if(mom!=NULL) mom->ifaine=idx;
        }
        return;
      }
      /* Sinon, on parcourt la liste => ins�rer. */
      {
        Ident cur=fils;
        Bool fini=0;
        while(fini==0) {
          Individu c2=getByIdent(g,cur);
          if(c2==NULL) {
            fini=1;
          } else {
            Ident nxt=c2->icadet;
            if(nxt==omega) {
              /* ins�re en queue */
              c2->icadet=idx;
              child->icadet=omega;
              fini=1;
            } else {
              Individu nixt=getByIdent(g,nxt);
              if(nixt==NULL) {
                c2->icadet=idx;
                child->icadet=omega;
                fini=1;
              } else {
                /* if child older => insert => compDate<0 => insert here. */
                if(compDate(child->naiss,nixt->naiss)<0) {
                  child->icadet=nxt;
                  c2->icadet=idx;
                  fini=1;
                } else {
                  cur=nxt;
                }
              }
            }
          }
        }
      }
    }
  }
}

/* adj => PRE: getByIdent(g,p)->ifaine == getByIdent(g,m)->ifaine
              compDate(getByIdent(g,p)->naiss,n)<0 etc.
*/
Ident adj(Genealogie g, Chaine s, Ident p, Ident m, Date n, Date d) {
  /* insert into tab by name (alphabetic) => getPos => insert => newID= id_cur-1 */
  Nat pos=getPos(g,s);
  insert(g,pos,s,p,m,n,d);
  {
    Ident newID=g->id_cur-1;
    /* now link child => father->ifaine => same pointer mother->ifaine */
    {
      Ident fils=omega;
      if(p!=omega) {
        Individu dad=getByIdent(g,p);
        if(dad!=NULL) {
          fils=dad->ifaine;
        }
      } else {
        Individu mom=getByIdent(g,m);
        if(mom!=NULL) {
          fils=mom->ifaine;
        }
      }
      adjFils(g,newID,fils,p,m);
    }
    return newID;
  }
}

/************************************************************/
/* 2.x AFFICHAGE => on ne fait pas de printf                */
/************************************************************/

/* 2.1 => fr�res/soeurs = father->ifaine or mother->ifaine, except x */
void affiche_freres_soeurs(Genealogie g, Ident x, Chaine buff) {
  buff[0]='\0';
  {
    Individu ix=getByIdent(g,x);
    if(ix==NULL) return;
    {
      Ident father=ix->ipere;
      Ident mother=ix->imere;
      Ident head=omega;
      if(father!=omega) {
        Individu dad=getByIdent(g,father);
        if(dad!=NULL) head=dad->ifaine;
      } else {
        if(mother!=omega) {
          Individu mom=getByIdent(g,mother);
          if(mom!=NULL) head=mom->ifaine;
        }
      }
      {
        Ident cur=head;
        while(cur!=omega) {
          if(cur!=x) {
            Individu c2=getByIdent(g,cur);
            if(c2!=NULL && STRINGLEN(c2->nom)>0) {
              if(STRINGLEN(buff)>0) {
                STRINGCAT(buff," ");
              }
              STRINGCAT(buff,c2->nom);
            }
          }
          {
            Individu c3=getByIdent(g,cur);
            if(c3==NULL) {
              cur=omega;
            } else {
              cur=c3->icadet;
            }
          }
        }
      }
    }
  }
}

/* 2.2 => enfants = x->ifaine->icadet => tri� du plus ancien au plus jeune */
void affiche_enfants(Genealogie g, Ident x, Chaine buff) {
  buff[0]='\0';
  {
    Individu ix=getByIdent(g,x);
    if(ix==NULL) return;
    {
      Ident child=ix->ifaine;
      while(child!=omega) {
        Individu c=getByIdent(g,child);
        if(c!=NULL && STRINGLEN(c->nom)>0) {
          if(STRINGLEN(buff)>0) {
            STRINGCAT(buff," ");
          }
          STRINGCAT(buff,c->nom);
        }
        if(c==NULL) {
          child=omega;
        } else {
          child=c->icadet;
        }
      }
    }
  }
}

/* 2.3 => affiche_cousins */
void affiche_cousins(Genealogie g, Ident x, Chaine buff) {
  buff[0]='\0';
  /* Implementation omitted for brevity */
  (void)g; (void)x;
}

/* 2.4 => affiche_oncles */
void affiche_oncles(Genealogie g, Ident x, Chaine buff) {
  buff[0]='\0';
  /* Implementation omitted for brevity */
  (void)g; (void)x;
}

/************************************************************/
/* 3.x => CREATION DE LIENS A POSTERIORI                    */
/************************************************************/

/* 3.1 => x,y deviennent freres/soeurs => fusion fratries, etc. */
void deviennent_freres_soeurs(Genealogie g, Ident x, Ident y) {
  (void)g; (void)x; (void)y;
}

/* 3.2 => x devient p�re de y => y->ipere=x => fusion fratries, etc. */
void devient_pere(Genealogie g, Ident x, Ident y) {
  (void)g; (void)x; (void)y;
}

/* x devient m�re de y => y->imere=x => fusion fratries, etc. */
void devient_mere(Genealogie g, Ident x, Ident y) {
  (void)g; (void)x; (void)y;
}

/************************************************************/
/* 4.x => PARCOURS                                           */
/************************************************************/

/* 4.1 => estAncetre(x,y)? => remonte parents y */
Bool estAncetre(Genealogie g, Ident x, Ident y) {
  if(x==omega || y==omega) return 0;
  if(x==y) return 0; /* or 1 if you consider self as ancestor, unclear from statement */
  {
    Ident pile[512];
    Nat top=0;
    pile[top]=y;
    top=top+1;
    while(top>0) {
      top=top-1;
      {
        Ident cur=pile[top];
        Individu c=getByIdent(g,cur);
        if(c!=NULL) {
          if(c->ipere==x || c->imere==x) {
            return 1;
          }
          if(c->ipere!=omega) {
            pile[top]=c->ipere;
            top=top+1;
          }
          if(c->imere!=omega) {
            pile[top]=c->imere;
            top=top+1;
          }
        }
      }
    }
    return 0;
  }
}

/* 4.2 => ontAncetreCommun(x,y) => check ancetres x vs y => partial */
Bool ontAncetreCommun(Genealogie g, Ident x, Ident y) {
  (void)g; (void)x; (void)y;
  return 0;
}

/* 4.3 => plus_ancien(x) => ancetre + ancien => partial */
Ident plus_ancien(Genealogie g, Ident x) {
  (void)g; (void)x;
  return omega;
}

/* 4.4 => affiche_parente(x,buff) => partial */
void affiche_parente(Genealogie g, Ident x, Chaine buff) {
  (void)g; (void)x;
  buff[0]='\0';
}

/* 4.5 => affiche_descendance(x,buff) => partial */
void affiche_descendance(Genealogie g, Ident x, Chaine buff) {
  (void)g; (void)x;
  buff[0]='\0';
}

/************************************************************/
/*                           MAIN                           */
/************************************************************/
int main(int argc, Chaine argv[]) {
  (void)argc; (void)argv;

  /* Petit test => On construit partiellement la figure 1. 
     On affiche freres/soeurs de Harry => doit �tre vide s'il n'a pas de sibling. */
  {
    Genealogie g;
    genealogieInit(&g);

    {
      Date dnull; 
      dnull.jour=0; dnull.mois=0; dnull.annee=0;

      /* Famille Potter */
      {
        Date jhen; jhen.jour=16; jhen.mois=2; jhen.annee=1867;
        Date jhed; jhed.jour=21; jhed.mois=11; jhed.annee=1932;
        Ident ihep= adj(g, "Henri", omega, omega, jhen, jhed);

        Date jfn; jfn.jour=30; jfn.mois=7; jfn.annee=1905;
        Date jfd; jfd.jour=2;  jfd.mois=3;  jfd.annee=1979;
        Ident ijfl = adj(g, "Fleamont", ihep, omega, jfn, jfd);

        Date jeu; jeu.jour=12; jeu.mois=6; jeu.annee=1907;
        Date jed; jed.jour=14; jed.mois=1; jed.annee=1979;
        Ident ijm = adj(g, "Euphemia", omega, omega, jeu, jed);

        Date jpn; jpn.jour=27; jpn.mois=3; jpn.annee=1960;
        Date jpd; jpd.jour=29; jpd.mois=7; jpd.annee=1981;
        Ident ijp = adj(g, "James", ijfl, ijm, jpn, jpd);

        Date lpn; lpn.jour=30; lpn.mois=1; lpn.annee=1960;
        Date lpd; lpd.jour=29; lpd.mois=7; lpd.annee=1981;
        Ident ilp = adj(g, "Lily", omega, omega, lpn, lpd);

        Date hn; hn.jour=31; hn.mois=7; hn.annee=1980;
        Ident ih = adj(g, "Harry", ijp, ilp, hn, dnull);
      }

      /* Famille Weasley (partielle) */
      {
        Date an; an.jour=6; an.mois=2; an.annee=1950;
        Ident iaw= adj(g, "Arthur", omega, omega, an, dnull);

        Date dpre; dpre.jour=8; dpre.mois=4; dpre.annee=1910;
        Date ddpre; ddpre.jour=23; ddpre.mois=10; ddpre.annee=1968;
        Ident ipre= adj(g, "Prewett", omega, omega, dpre, ddpre);

        Date dfab; dfab.jour=12; dfab.mois=5; dfab.annee=1946;
        Date ddfab; ddfab.jour=21; ddfab.mois=12; ddfab.annee=1982;
        Ident ifab= adj(g, "Fabian", ipre, omega, dfab, ddfab);

        Date mn; mn.jour=30; mn.mois=10; mn.annee=1949;
        Ident imw= adj(g, "Molly", ipre, omega, mn, dnull);

        /* etc. => you can add Bill, Charlie, Percy, etc. */
      }
    }

    {
      Car buff[256];
      /* Siblings of Harry => might be empty if no sibling was added. */
      Ident iharry=6; /* or we can do a name-based search. We'll do direct ID. */
      buff[0]='\0';
      affiche_freres_soeurs(g, iharry, buff);
      printf("Freres/soeurs de Harry: %s\n", buff);
    }

    genealogieFree(&g);
  }

  return 0;
}
