#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct _stablo;
typedef struct _stablo* stabloP;
typedef struct _stablo {
    int element;
    stabloP lijevo;
    stabloP desno;
} stablo;

stabloP UnesiElement(int element, stabloP root);
stabloP NoviElement(int element);
int IspisInorder(stabloP root);
int IspisPreorder(stabloP root);
int IspisPostorder(stabloP root);
stabloP PronadjiElement(int element, stabloP root);
stabloP PronadjiMinElement(stabloP root);
stabloP IzbrisiElement(int element, stabloP root);

int main()
{
    stabloP korijen = NULL;
    stabloP temp = NULL;
    int izbor = 0;
    int element = 0;

    do {
        printf("\n*************************************\n");
        printf("Odaberi: \n");
        printf("1 - Unos elementa u stablo\n");
        printf("2 - Ispis stabla (inorder)\n");
        printf("3 - Ispis stabla (preorder)\n");
        printf("4 - Ispis stabla (postorder)\n");
        printf("5 - Pronadji element u stablu\n");
        printf("6 - Izbrisi element iz stabla\n");
        printf("0 - Izlaz\n");
        printf("\n*************************************\n");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1:
            printf("Unesi element za stablo: ");
            scanf("%d", &element);
            korijen = UnesiElement(element, korijen);
            break;
        case 2:
            IspisInorder(korijen);
            break;
        case 3:
            IspisPreorder(korijen);
            break;
        case 4:
            IspisPostorder(korijen);
            break;
        case 5:
            printf("Unesi element koji zelis pronaci u stablu: ");
            scanf("%d", &element);
            temp = PronadjiElement(element, korijen);
            if (temp != NULL) {
                printf("Element %d je pronadjen u stablu\n", temp->element);
            }
            else {
                printf("Element %d nije pronadjen u stablu\n", element);
            }
            break;
        case 6:
            printf("Unesi element koji zelis izbrisati iz stabla: ");
            scanf("%d", &element);
            korijen = IzbrisiElement(element, korijen);
            break;
        case 0:
            break;
        default:
            printf("Neispravan unos!\n");
        }
    } while (izbor != 0);

    return 0;
}

stabloP UnesiElement(int element, stabloP root)
{
    if (root == NULL) {
        return NoviElement(element);
    }
    if (element < root->element) {
        root->lijevo = UnesiElement(element, root->lijevo);
    }
    else if (element > root->element) {
        root->desno = UnesiElement(element, root->desno);
    }
    return root;
}

stabloP NoviElement(int element)
{
    stabloP novi = (stabloP)malloc(sizeof(stablo));
    if (novi == NULL) {
        printf("Nema dovoljno memorije!\n");
        return NULL;
    }
    novi->element = element;
    novi->lijevo = NULL;
    novi->desno = NULL;
    return novi;
}

int IspisInorder(stabloP root)
{
    if (root != NULL) {
        IspisInorder(root->lijevo);
        printf("%d ", root->element);
        IspisInorder(root->desno);
    }
    return 0;
}

int IspisPreorder(stabloP root)
{
    if (root != NULL) {
        printf("%d ", root->element);
        IspisPreorder(root->lijevo);
        IspisPreorder(root->desno);
    }
    return 0;
}

int IspisPostorder(stabloP root)
{
    if (root != NULL) {
        IspisPostorder(root->lijevo);
        IspisPostorder(root->desno);
        printf("%d ", root->element);
    }
    return 0;
}

stabloP PronadjiElement(int element, stabloP root)
{
    if (root == NULL) {
        return NULL;
    }
    if (element < root->element) {
        return PronadjiElement(element, root->lijevo);
    }
    else if (element > root->element) {
        return PronadjiElement(element, root->desno);
    }
    else {
        return root;
    }
}

stabloP PronadjiMinElement(stabloP root)
{
    while (root->lijevo != NULL) {
        root = root->lijevo;
    }
    return root;
}

stabloP IzbrisiElement(int element, stabloP root)
{
    if (root == NULL) {
        printf("Element nije pronadjen u stablu!\n");
        return NULL;
    }

    if (element < root->element) {
        root->lijevo = IzbrisiElement(element, root->lijevo);
    }
    else if (element > root->element) {
        root->desno = IzbrisiElement(element, root->desno);
    }
    else {
        if (root->lijevo == NULL && root->desno == NULL) {
            free(root);
            return NULL;
        }
        else if (root->lijevo == NULL) {
            stabloP temp = root->desno;
            free(root);
            return temp;
        }
        else if (root->desno == NULL) {
            stabloP temp = root->lijevo;
            free(root);
            return temp;
        }
        else {
            stabloP temp = PronadjiMinElement(root->desno);
            root->element = temp->element;
            root->desno = IzbrisiElement(temp->element, root->desno);
        }
    }
    return root;
}
