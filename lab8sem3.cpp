#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <list>
#include <queue>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct element
{
    int znach;
    struct element* dalshe;
};

struct element* sozdat_element(int chislo) {
    struct element* ukazatel = (struct element*)malloc(sizeof(struct element));
    if (ukazatel == NULL) {
        printf("Память плохая\n");
        return NULL;
    }
    ukazatel->znach = chislo;
    ukazatel->dalshe = NULL;
    return ukazatel;
}

void dobavit_v_ochered(struct element** start, struct element** konec, int chislo) {
    struct element* noviy = sozdat_element(chislo);
    if (noviy == NULL) return;

    if (*konec == NULL) {
        *start = noviy;
        *konec = noviy;
    }
    else {
        (*konec)->dalshe = noviy;
        *konec = noviy;
    }
}

int udalit_iz_ocheredi(struct element** start, struct element** konec) {
    if (*start == NULL) {
        return -1;
    }

    struct element* temp = *start;
    int chislo = temp->znach;
    *start = (*start)->dalshe;

    if (*start == NULL) {
        *konec = NULL;
    }

    free(temp);
    return chislo;
}

int ochered_pustaya(struct element* start) {
    return start == NULL;
}

void ochistit_ochered(struct element** start, struct element** konec) {
    while (*start != NULL) {
        udalit_iz_ocheredi(start, konec);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(NULL));

    int kolvo;
    printf("Сколько вершин в графе: ");
    while (scanf("%d", &kolvo) != 1 || kolvo <= 0) {
        printf("Ошибка! Введите положительное число: ");
        while (getchar() != '\n');
    }

    vector<vector<int>> matrica(kolvo, vector<int>(kolvo));
    vector<list<int>> spisok_smejnosti(kolvo);

    for (int i = 0; i < kolvo; i++) {
        for (int j = i; j < kolvo; j++) {
            if (i == j) {
                matrica[i][j] = 0;
            }
            else {
                matrica[i][j] = matrica[j][i] = rand() % 2;
            }
        }
    }

    printf("Матрица смежности:\n");
    for (int i = 0; i < kolvo; i++) {
        for (int j = 0; j < kolvo; j++) {
            printf("%d ", matrica[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < kolvo; i++) {
        for (int j = 0; j < kolvo; j++) {
            if (matrica[i][j] == 1) {
                spisok_smejnosti[i].push_back(j);
            }
        }
    }

    printf("\nСписки смежности:\n");
    for (int i = 0; i < kolvo; i++) {
        printf("%d: ", i + 1);
        for (int sosed : spisok_smejnosti[i]) {
            printf("%d ", sosed + 1);
        }
        printf("\n");
    }

    vector<bool> poseshcheni(kolvo, false);
    queue<int> standartnaya_ochered;

    printf("\nОбход в ширину (матрица, std::queue): ");
    for (int i = 0; i < kolvo; i++) {
        if (!poseshcheni[i]) {
            standartnaya_ochered.push(i);
            poseshcheni[i] = true;

            while (!standartnaya_ochered.empty()) {
                int tekushaya = standartnaya_ochered.front();
                standartnaya_ochered.pop();
                printf("%d ", tekushaya + 1);

                for (int j = 0; j < kolvo; j++) {
                    if (matrica[tekushaya][j] == 1 && !poseshcheni[j]) {
                        standartnaya_ochered.push(j);
                        poseshcheni[j] = true;
                    }
                }
            }
        }
    }

    fill(poseshcheni.begin(), poseshcheni.end(), false);
    printf("\nОбход в ширину (списки, std::queue): ");

    for (int i = 0; i < kolvo; i++) {
        if (!poseshcheni[i]) {
            standartnaya_ochered.push(i);
            poseshcheni[i] = true;

            while (!standartnaya_ochered.empty()) {
                int tekushaya = standartnaya_ochered.front();
                standartnaya_ochered.pop();
                printf("%d ", tekushaya + 1);

                for (int sosed : spisok_smejnosti[tekushaya]) {
                    if (!poseshcheni[sosed]) {
                        standartnaya_ochered.push(sosed);
                        poseshcheni[sosed] = true;
                    }
                }
            }
        }
    }

    fill(poseshcheni.begin(), poseshcheni.end(), false);
    struct element* moya_ochered_start = NULL, * moya_ochered_konec = NULL;

    printf("\nОбход в ширину (матрица, своя очередь): ");

    for (int i = 0; i < kolvo; i++) {
        if (!poseshcheni[i]) {
            dobavit_v_ochered(&moya_ochered_start, &moya_ochered_konec, i);
            poseshcheni[i] = true;

            while (!ochered_pustaya(moya_ochered_start)) {
                int tekushaya = udalit_iz_ocheredi(&moya_ochered_start, &moya_ochered_konec);
                printf("%d ", tekushaya + 1);

                for (int j = 0; j < kolvo; j++) {
                    if (matrica[tekushaya][j] == 1 && !poseshcheni[j]) {
                        dobavit_v_ochered(&moya_ochered_start, &moya_ochered_konec, j);
                        poseshcheni[j] = true;
                    }
                }
            }
        }
    }

    fill(poseshcheni.begin(), poseshcheni.end(), false);
    ochistit_ochered(&moya_ochered_start, &moya_ochered_konec);

    printf("\nОбход в ширину (списки, своя очередь): ");

    for (int i = 0; i < kolvo; i++) {
        if (!poseshcheni[i]) {
            dobavit_v_ochered(&moya_ochered_start, &moya_ochered_konec, i);
            poseshcheni[i] = true;

            while (!ochered_pustaya(moya_ochered_start)) {
                int tekushaya = udalit_iz_ocheredi(&moya_ochered_start, &moya_ochered_konec);
                printf("%d ", tekushaya + 1);

                for (int sosed : spisok_smejnosti[tekushaya]) {
                    if (!poseshcheni[sosed]) {
                        dobavit_v_ochered(&moya_ochered_start, &moya_ochered_konec, sosed);
                        poseshcheni[sosed] = true;
                    }
                }
            }
        }
    }

    ochistit_ochered(&moya_ochered_start, &moya_ochered_konec);

    int min_razmer, max_razmer, shag;

    printf("\n\nНастройка тестов:\n");

    printf("Минимальный размер графа: ");
    while (scanf("%d", &min_razmer) != 1 || min_razmer <= 0) {
        printf("Ошибка! Введите положительное число: ");
        while (getchar() != '\n');
    }

    printf("Максимальный размер графа: ");
    while (scanf("%d", &max_razmer) != 1 || max_razmer <= min_razmer) {
        printf("Ошибка! Введите число больше %d: ", min_razmer);
        while (getchar() != '\n');
    }

    printf("Шаг тестирования: ");
    while (scanf("%d", &shag) != 1 || shag <= 0) {
        printf("Ошибка! Введите положительное число: ");
        while (getchar() != '\n');
    }

    vector<int> razmeri;
    for (int razmer = min_razmer; razmer <= max_razmer; razmer += shag) {
        razmeri.push_back(razmer);
    }

    printf("Будет проведено %d тестов от %d до %d с шагом %d...\n",
        (int)razmeri.size(), min_razmer, max_razmer, shag);

    printf("\nСравнение времени работы:\n");
    printf("Размер  | Матрица+std | Матрица+своя | Списки+std | Списки+своя | Победитель\n");
    printf("----------------------------------------------------------------------------\n");

    vector<int> pobediteli(4, 0);

    for (int razmer : razmeri) {
        printf("Тестируем размер %d... ", razmer);
        fflush(stdout);

        vector<vector<int>> test_matrica(razmer, vector<int>(razmer));
        vector<list<int>> test_spiski(razmer);

        for (int i = 0; i < razmer; i++) {
            for (int j = i; j < razmer; j++) {
                if (i == j) {
                    test_matrica[i][j] = 0;
                }
                else {
                    test_matrica[i][j] = test_matrica[j][i] = rand() % 2;
                    if (test_matrica[i][j] == 1) {
                        test_spiski[i].push_back(j);
                        test_spiski[j].push_back(i);
                    }
                }
            }
        }

        vector<long long> vremena(4);

        vector<bool> test_poseshcheni(razmer, false);
        auto nachalo1 = high_resolution_clock::now();
        queue<int> std_ochered;
        for (int i = 0; i < razmer; i++) {
            if (!test_poseshcheni[i]) {
                std_ochered.push(i);
                test_poseshcheni[i] = true;
                while (!std_ochered.empty()) {
                    int tekushaya = std_ochered.front();
                    std_ochered.pop();
                    for (int j = 0; j < razmer; j++) {
                        if (test_matrica[tekushaya][j] == 1 && !test_poseshcheni[j]) {
                            std_ochered.push(j);
                            test_poseshcheni[j] = true;
                        }
                    }
                }
            }
        }
        auto konec1 = high_resolution_clock::now();
        vremena[0] = duration_cast<microseconds>(konec1 - nachalo1).count();

        fill(test_poseshcheni.begin(), test_poseshcheni.end(), false);
        struct element* test_start1 = NULL, * test_konec1 = NULL;
        auto nachalo2 = high_resolution_clock::now();
        for (int i = 0; i < razmer; i++) {
            if (!test_poseshcheni[i]) {
                dobavit_v_ochered(&test_start1, &test_konec1, i);
                test_poseshcheni[i] = true;
                while (!ochered_pustaya(test_start1)) {
                    int tekushaya = udalit_iz_ocheredi(&test_start1, &test_konec1);
                    for (int j = 0; j < razmer; j++) {
                        if (test_matrica[tekushaya][j] == 1 && !test_poseshcheni[j]) {
                            dobavit_v_ochered(&test_start1, &test_konec1, j);
                            test_poseshcheni[j] = true;
                        }
                    }
                }
            }
        }
        auto konec2 = high_resolution_clock::now();
        vremena[1] = duration_cast<microseconds>(konec2 - nachalo2).count();
        ochistit_ochered(&test_start1, &test_konec1);

        fill(test_poseshcheni.begin(), test_poseshcheni.end(), false);
        auto nachalo3 = high_resolution_clock::now();
        queue<int> std_ochered2;
        for (int i = 0; i < razmer; i++) {
            if (!test_poseshcheni[i]) {
                std_ochered2.push(i);
                test_poseshcheni[i] = true;
                while (!std_ochered2.empty()) {
                    int tekushaya = std_ochered2.front();
                    std_ochered2.pop();
                    for (int sosed : test_spiski[tekushaya]) {
                        if (!test_poseshcheni[sosed]) {
                            std_ochered2.push(sosed);
                            test_poseshcheni[sosed] = true;
                        }
                    }
                }
            }
        }
        auto konec3 = high_resolution_clock::now();
        vremena[2] = duration_cast<microseconds>(konec3 - nachalo3).count();

        fill(test_poseshcheni.begin(), test_poseshcheni.end(), false);
        struct element* test_start2 = NULL, * test_konec2 = NULL;
        auto nachalo4 = high_resolution_clock::now();
        for (int i = 0; i < razmer; i++) {
            if (!test_poseshcheni[i]) {
                dobavit_v_ochered(&test_start2, &test_konec2, i);
                test_poseshcheni[i] = true;
                while (!ochered_pustaya(test_start2)) {
                    int tekushaya = udalit_iz_ocheredi(&test_start2, &test_konec2);
                    for (int sosed : test_spiski[tekushaya]) {
                        if (!test_poseshcheni[sosed]) {
                            dobavit_v_ochered(&test_start2, &test_konec2, sosed);
                            test_poseshcheni[sosed] = true;
                        }
                    }
                }
            }
        }
        auto konec4 = high_resolution_clock::now();
        vremena[3] = duration_cast<microseconds>(konec4 - nachalo4).count();
        ochistit_ochered(&test_start2, &test_konec2);

        int pobeditel = min_element(vremena.begin(), vremena.end()) - vremena.begin();
        pobediteli[pobeditel]++;

        const char* nazvaniya[] = { "Матрица+std", "Матрица+своя", "Списки+std", "Списки+своя" };

        printf("\r%7d | %11lld | %12lld | %10lld | %11lld | %-12s\n",
            razmer, vremena[0], vremena[1], vremena[2], vremena[3], nazvaniya[pobeditel]);
    }

    printf("\nТОП методов по скорости:\n");
    printf("========================\n");
    const char* nazvaniya[] = { "Матрица+std", "Матрица+своя", "Списки+std", "Списки+своя" };

    vector<pair<int, int>> reyting;
    for (int i = 0; i < 4; i++) {
        reyting.push_back({ pobediteli[i], i });
    }

    sort(reyting.rbegin(), reyting.rend());

    for (int i = 0; i < 4; i++) {
        printf("%d место: %s - %d побед\n",
            i + 1, nazvaniya[reyting[i].second], reyting[i].first);
    }

    return 0;
}