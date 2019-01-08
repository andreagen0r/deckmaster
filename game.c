#include "game.h"

static unsigned int m_render_mode = COLOR_MODE;

// Menu principal do jogo, ponto de inínico de tudo
void game_init(void);

// Seta as variáveis e chama o loop principal do jogo
void make_game(void);

void set_players_name(Player *m_player1, Player *m_player2);

// Menu para selecionar a opção de visualização
void render_mode_menu(void);

// Loop principal do jogo
void game_main_loop(Board *m_board, Pack *m_pack, Player *m_player1, Player *m_player2);

// Executa a ação de fusão das cartas quando um jogador sobreescrever uma carta existente no tabuleiro
void fusion(Board *m_board, const Card next_card, const Point m_position);

// Retorna a posição no tabuleiro entre 0-9
int get_case(Point m_position);

// Função para encontrar as possíveis direções de ataque
void get_direction(const Board *m_board, Point m_position, const int *possible_directions,
         int *directions, const size_t loop );

// Fução geral do atack chamada dentro do loop principal
void make_attack(Board *m_board, Player *m_last_player, Point m_attacker);

// Função que é chamada para cada caso de ataque.
// Dentro dela é chamado (get_direction()) para saber qual carta atacar
// Depois chama a função de ataque que realiza todos as verificações para contagem dos pontos
void attack_case(Board *m_board, Player *m_player, const Point m_attacker,
            const Point *m_defender, const int *m_possible_directions, int *m_directions, const size_t m_loop);

// Função que efetivamente faz o ataque e conta os pontos de acordo com todas as regras de direções das setas
// e dos elementos envolvidos no ataque
void attack(Board *m_board, Player *m_last_player, Point m_attacker, Point m_defender);

// Percorre todas as casas do tabuleiro e elimina as cartas com status ENVENENADO
void poisoned_damage(Board *m_board);

// Retorna true se a carta de defesa possuir seta apontando para carta de ataque, senão retorna false
bool is_match_arrow(Board *m_board, Point m_attacker, Point m_defense);

//
bool defense_case(Board *m_board, const Point m_attacker, const Point m_defender,
            const Point *m_attacks, const int *m_possible_directions, int *m_directions, const size_t m_loop);

// Faz a contagem padrão dos pontos
void score_case_default(Board *m_board, const Point m_attacker, const Point m_defender);

// Faz a contagem de pontos seguindo o segundo caso
void score_case1(Board *m_board, const Point m_attacker, const Point m_defender);

// Faz a contagem de pontos seguindo o terceiro caso
void score_case2(Board *m_board, const Point m_defender);

// Altera o status da carta atacada para um dos possíveis estados
void elemental_damage(Board *m_board, const Point m_attacker, const Point m_defender);

// Exibe o jogo na tela
void render(const Board *m_board, Pack *m_pack,
            const Player *m_player1, const Player *m_player2);

// Imprime o placar dos jogadores
void print_score(const Player *m_player1, const Player *m_player2);

// Imprime placar final com os resultados
void print_scoreboard(const Player *m_player1, const Player *m_player2);

// Exibe logo e mensagem de boas vindas
void deckmaster_logo(unsigned int m_render);

void game_init()
{
    bool m_exit = false;

    do {
        size_t option = 0;

        limparTela();
        deckmaster_logo(TEXT_MODE);

        puts("\n");
        puts("(0): Iniciar o jogo");
        puts("(1): Configurar modo de visualização");
        puts("(9): Sair\n");

        if (option > 1)
        {
            puts("\n");
            puts("**** Opção invávida, escolha uma opção do menu ****");
        }

        printf("Digite uma opção: ");
        scanf("%zd", &option);

        switch (option) {
        case 0:
            make_game();
            break;

        case 1:
            render_mode_menu();
            break;

        case 9:
            m_exit = true;
            limparTela();
            puts("Obrigado por jogar Deckmaster, volte sempre!\n");
            exit(1);
        }

    } while (!m_exit);
}

void make_game()
{
    // Define a semente para geração de valores aleatórios
    srand( (unsigned)time(NULL) );

    // Inicializa o tabuleiro
    Board m_board;
    board_init(&m_board);

    Pack *m_pack = NULL;
    // Inicializa o baralho
    m_pack = pack_init();

    Player m_player1;
    Player m_player2;

    set_players_name(&m_player1, &m_player2);

    // Inicia o jogo
    game_main_loop(&m_board, m_pack, &m_player1, &m_player2);
}

void set_players_name(Player *m_player1, Player *m_player2)
{
    limparTela();

    deckmaster_logo(m_render_mode);

    // Inicializa os jogadores com seus respectivos nomes
    char tmp[100];
    puts("\n");
    printf("Digite o nome do primeiro jogador: ");
    scanf("%s", tmp);
    player_init(m_player1, tmp);

    puts("\n");
    printf("Digite o nome do segundo jogador: ");
    scanf("%s", tmp);
    player_init(m_player2, tmp);
}

void render_mode_menu()
{
    do {
        limparTela();

        deckmaster_logo(m_render_mode);

        puts("\n");
        puts("Selecione o modo de visualização\n");
        puts("Modo texto = 0");
        puts("Modo cores = 1 <padrão>");

        if (m_render_mode > 1)
        {
            puts("\n");
            puts("**** Opção invávida, escolha uma opção do menu ****");
        }
        printf("\n>");
        scanf("%d", &m_render_mode);

    } while (m_render_mode > 1);
}

void game_main_loop(Board *m_board, Pack *m_pack, Player *m_player1, Player *m_player2)
{
    bool end_game = false;
    bool player = true;
    bool board_space_allowed = true;

    char *player_time_name = NULL;

    char color_alert[8] = "";
    char color_line[8] = "";
    char color_column[8] = "";
    char color_reset[8] = "";

    if (m_render_mode == COLOR_MODE)
    {
        memcpy(color_alert, "\x1b[31m", sizeof(color_alert));
        memcpy(color_line, "\x1b[35m", sizeof(color_alert));
        memcpy(color_column, "\x1b[36m", sizeof(color_alert));
        memcpy(color_reset, "\x1b[0m", sizeof(color_alert));
    }

    Point position;

    render(m_board, m_pack, m_player1, m_player2);

    // Loop principal do jogo
    do {
        // Alterna o nome do jogador da vez
        if (player)
        {
            player_time_name = m_player1->name;
            player = false;
        }
        else
        {
            player_time_name = m_player2->name;
            player = true;
        }


        // Fica em loop até que a possição seja válida
        do {
            // Imprime mensagem caso a posição solicitada não for permitida
            if (!board_space_allowed)
            {
                printf("%s**** Não é possível jogar na posição [%d, %d], tente novamente ******\n%s",
                       color_alert, position.x, position.y, color_reset);
            }

            // Solicita a digitação da coordenada que deseja jogar a carta
            printf("%s digite a <%slinha%s e %scoluna%s>: ", player_time_name,
                   color_line, color_reset, color_column, color_reset);

            scanf("%d %d", &position.x, &position.y );


            // Compra carta do topo do baralho
            Card next_card = pack_top(m_pack);

            if (!board_isFull(m_board))
            {
                // tenta Jogar carta no tabuleiro
                board_space_allowed = board_add_card(m_board, next_card, position);
            }
            else
            {
                // Fusion
                fusion(m_board, next_card, position);
            }



            if (board_space_allowed)
            {
                // Remove a carta do topo do baralho
                pack_pop(&m_pack);

                // Efetua o ataque se a posição escolhida for válida
                make_attack(m_board, (!player) ? m_player1 : m_player2, position);
            }

            render(m_board, m_pack, m_player1, m_player2);

            // Se não houver mais cartas no baralho, encerra o jogo
            end_game = (pack_size() == 0) ? true : false;

        } while (!board_space_allowed);

    } while (!end_game);


    print_scoreboard(m_player1, m_player2);

}

void fusion(Board *m_board, const Card next_card, const Point m_position)
{
    /*
     * FUSÃO DE CARTAS
     * Uma carta posicionada em um espaço do tabuleiro já ocupado por outra gera uma nova carta através da “fusão” de
     * ambas, sendo o valor do atributo poder da nova carta o resultado da soma dos poderes das cartas fundidas.
     * Todos os outros atributos, no entanto, são iguais aos atributos da última carta jogada.
    */

    Card fused_card = card_fusion(next_card, m_board->position[m_position.x][m_position.y]);

    // Remove a carta na posição do tabuleiro
    board_remove_card(m_board, m_position);

    // Adiciona a nova carta na posição do tabuleiro
    board_add_card(m_board, fused_card, m_position);
}

int get_case(Point m_position)
{
    int out = -1;

    int count = 0;

    for (int row = 0; row < BASE_GRID_SIZE; ++row)
    {
        for (int col = 0; col < BASE_GRID_SIZE; ++col)
        {
            if (m_position.x == row && m_position.y == col)
            {
                out = count;
                return  out;
            }

            count++;
        }
    }

    return out;
}

void get_direction(const Board *m_board, const Point m_position,
                    const int *possible_directions, int *directions, const size_t m_loop)
{
    for (size_t i = 0; i < m_loop; ++i)
    {
        // Verifica se a direção de defesa é uma das posiveis
        if (m_board->position[m_position.x][m_position.y].arrows[possible_directions[i]])
        {
            // Guarda a posição de defesa
            directions[i] = possible_directions[i];
        }
        else
        {
            directions[i] = NO_DIRECTION;
        }
    }
}

void make_attack(Board *m_board, Player *m_last_player, Point m_position)
{
    /*
     * ATAQUE
     * As cartas posicionadas nos espaços adjacentes às orientações de ataque da carta
     * recém posicionada sofrem dano – isto é, têm seu poder decrementado – com base no
     * poder da carta jogada.
     *
     * O cálculo é realizado de uma das seguintes formas:
     *  i. A carta atacada NÃO possui uma orientação de ataque oposta à da atacante:
     *      Nesse caso, a carta atacante subtrai do poder da carta atacada uma quantia igual
     *      ao seu próprio poder. No exemplo abaixo, se a carta à direita é a atacante, então a carta
     *      a esquerda terá seu poder reduzido de 5 para 1:

                           ---------------          ---------------
                           |      ^      |          |      ^      |
                           |             |          |             |
                           | <    5      |          | <    4    > |
                           |             |  <----   |             |
                           |    FOGO     |          |    FOGO     |
                           |             |          |             |
                           |      v      |          |      v      |
                           ---------------          ---------------
                           Carta da esquerda sofre 4 pontos de dano


     *  ii. A carta atacada possui uma orientação de ataque oposta à da atacante:
     *
     *  (CASO 1) A carta atacante possui poder maior que a atacada:
     *
     *  Nesse caso, a carta atacante subtrai do poder da carta atacada uma
     *  quantia igual apenas a DIFERENÇA entre o seu próprio poder e o da carta
     *  atacada. No exemplo abaixo, se a carta à direita é a atacante, então a carta a
     *  esquerda terá seu poder reduzido de 4 para 3:
     *
                           ---------------          ---------------
                           |      ^      |          |      ^      |
                           |             |          |             |
                           | <    4    > |          | <    5    > |
                           |             |  <----   |             |
                           |    FOGO     |          |    FOGO     |
                           |             |          |             |
                           |             |          |      v      |
                           ---------------          ---------------
            Carta da esquerda sofre perde pontos de poder igual a diferença dos poderes de ambas
     *
     *
     * (CASO 2) A carta atacante possui poder menor ou igual a atacada:
     *
     * Nesse caso, a carta atacante subtrai do poder da carta atacada apenas 1
     * ponto. No exemplo abaixo, se a carta à direita é a atacante, então a carta a
     * esquerda terá seu poder reduzido de 5 para 4:
     *
                           ---------------          ---------------
                           |      ^      |          |      ^      |
                           |             |          |             |
                           | <    5    > |          | <    3    > |
                           |             |  <----   |             |
                           |    FOGO     |          |    FOGO     |
                           |             |          |             |
                           |             |          |      v      |
                           ---------------          ---------------
*/


    switch (get_case(m_position)) {
    case 0:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | A | D | * |
         * |---|---|---|
         * | D | * | * |
         * |---|---|---|
         * | * | * | * |
         * ---------------
         */

        // Define as posições da defesa
        const Point defenses[2] = { {0,1}, {1,0} };

        // Define as possíveis direções da ataque
        const int possible_directions[2] = {RIGHT, BOTTOM};

        // Armazenará as direções válidas
        int directions[2];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (defenses) / sizeof(defenses[0]);

        attack_case(m_board, m_last_player, m_position, defenses, possible_directions, directions, loop);
    }

        break;

    case 1:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | D | A | D |
         * |---|---|---|
         * | * | D | * |
         * |---|---|---|
         * | * | * | * |
         * ---------------
         * */

        // Define as posições da defesa
        Point defenses[3] = { {0,0}, {0,2}, {1,1} };

        // Define as possíveis direções do ataque
        const int possible_directions[3] = {LEFT, RIGHT, BOTTOM};

        // Armazenará as direções válidas
        int directions[3];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (defenses) / sizeof(defenses[0]);

        attack_case(m_board, m_last_player, m_position, defenses, possible_directions, directions, loop);
    }

        break;

    case 2:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | D | A |
         * |---|---|---|
         * | * | * | D |
         * |---|---|---|
         * | * | * | * |
         * ---------------
         * */

        // Define as posições da defesa
        Point defenses[2] = { {0,1}, {1,2} };

        // Define as possíveis direções do ataque
        const int possible_directions[2] = {LEFT, BOTTOM};

        // Armazenará as direções válidas
        int directions[2];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (defenses) / sizeof(defenses[0]);

        attack_case(m_board, m_last_player, m_position, defenses, possible_directions, directions, loop);
    }

        break;

    case 3:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | D | * | * |
         * |---|---|---|
         * | A | D | * |
         * |---|---|---|
         * | D | * | * |
         * ---------------
         * */

        // Define as posições da defesa
        Point defenses[3] = { {0,0}, {1,1}, {2,0} };

        // Define as possíveis direções do ataque
        const int possible_directions[3] = {TOP, RIGHT, BOTTOM};

        // Armazenará as direções válidas
        int directions[3];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (defenses) / sizeof(defenses[0]);

        attack_case(m_board, m_last_player, m_position, defenses, possible_directions, directions, loop);
    }

        break;

    case 4:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | D | * |
         * |---|---|---|
         * | D | A | D |
         * |---|---|---|
         * | * | D | * |
         * ---------------
         * */

        // Define as posições da defesa
        Point defenses[4] = { {0,1}, {1,0}, {1,2}, {2,1} };

        // Define as possíveis direções do ataque
        const int possible_directions[4] = {TOP, LEFT, RIGHT, BOTTOM};

        // Armazenará as direções válidas
        int directions[4];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (defenses) / sizeof(defenses[0]);

        attack_case(m_board, m_last_player, m_position, defenses, possible_directions, directions, loop);
    }

        break;

    case 5:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | * | D |
         * |---|---|---|
         * | * | D | A |
         * |---|---|---|
         * | * | * | D |
         * ---------------
         * */

        // Define as posições da defesa
        Point defenses[3] = { {0,2}, {1,1}, {2,2} };

        // Define as possíveis direções do ataque
        const int possible_directions[3] = {TOP, LEFT, BOTTOM};

        // Armazenará as direções válidas
        int directions[3];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (defenses) / sizeof(defenses[0]);

        attack_case(m_board, m_last_player, m_position, defenses, possible_directions, directions, loop);
    }

        break;

    case 6:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | * | * |
         * |---|---|---|
         * | D | * | * |
         * |---|---|---|
         * | A | D | * |
         * ---------------
         * */

        // Define as posições da defesa
        Point defenses[2] = { {1,0}, {2,1} };

        // Define as possíveis direções do ataque
        const int possible_directions[2] = {TOP, RIGHT};

        // Armazenará as direções válidas
        int directions[2];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (defenses) / sizeof(defenses[0]);

        attack_case(m_board, m_last_player, m_position, defenses, possible_directions, directions, loop);
    }

        break;

    case 7:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | * | * |
         * |---|---|---|
         * | * | D | * |
         * |---|---|---|
         * | D | A | D |
         * ---------------
         * */

        // Define as posições da defesa
        Point defenses[3] = { {1,1}, {2,0}, {2,2} };

        // Define as possíveis direções do ataque
        const int possible_directions[3] = {TOP, LEFT, RIGHT};

        // Armazenará as direções válidas
        int directions[3];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (defenses) / sizeof(defenses[0]);

        attack_case(m_board, m_last_player, m_position, defenses, possible_directions, directions, loop);
    }

        break;

    case 8:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | * | * |
         * |---|---|---|
         * | * | * | D |
         * |---|---|---|
         * | * | D | A |
         * ---------------
         * */

        // Define as posições da defesa
        Point defenses[2] = { {1,2}, {2,1} };

        // Define as possíveis direções do ataque
        const int possible_directions[2] = {TOP, LEFT};

        // Armazenará as direções válidas
        int directions[2];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (defenses) / sizeof(defenses[0]);

        attack_case(m_board, m_last_player, m_position, defenses, possible_directions, directions, loop);
    }

        break;

    default:
        puts("ERRO! -> (make_attack)");
        break;
    }
}

void attack_case(Board *m_board, Player *m_player, const Point m_attacker,
            const Point *m_defender, const int *m_possible_directions, int *m_directions, const size_t m_loop)
{
    get_direction(m_board, m_attacker, m_possible_directions, m_directions, m_loop);

    for (size_t row = 0; row < m_loop; ++row)
    {
        if (m_directions[row] != NO_DIRECTION)
        {
            if (!m_board->is_busy[m_defender[row].x][m_defender[row].y])
            {
                attack(m_board, m_player, m_attacker, m_defender[row]);
            }
        }
    }

    poisoned_damage(m_board);
}

void attack(Board *m_board, Player *m_player, Point m_attacker, Point m_defender)
{
    // Verifica se o status da carta atacada está congelado
    if (m_board->position[m_defender.x][m_defender.y].status == FROZEN)
    {
        score_case_default(m_board, m_attacker, m_defender);
    }
    else
    {
        // Defesa NÃO tem seta apontando para ataque
        if (!is_match_arrow(m_board, m_attacker, m_defender))
        {
            score_case_default(m_board, m_attacker, m_defender);
        }
        else
        {
            if (m_board->position[m_attacker.x][m_attacker.y].power > m_board->position[m_defender.x][m_defender.y].power)
            {
                score_case1(m_board, m_attacker, m_defender);
            }
            else
            {
                score_case2(m_board, m_defender);
            }
        }

    }

    // Dano causado pelo elemento FOGO
    if (m_board->position[m_defender.x][m_defender.y].status == BURNED)
    {
        int power = m_board->position[m_attacker.x][m_attacker.y].power;
        m_board->position[m_attacker.x][m_attacker.y].power = power - 1;

        if (m_board->position[m_attacker.x][m_attacker.y].power <= 0)
        {
            board_remove_card(m_board, m_attacker);
        }
    }


    // Tenta remover a carta
    if (m_board->position[m_defender.x][m_defender.y].power <= 0)
    {
        if (board_remove_card(m_board, m_defender))
        {
            // Adiciona ponto ao jogador que está atacando
            player_add_score(m_player, 1);
        }
    }

    elemental_damage(m_board, m_attacker, m_defender);
}

void poisoned_damage(Board *m_board)
{
    // Remove 1 ponto de poder de cada carta que estiver com status envenenado
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (m_board->position[row][col].status == POISONED)
            {
                int power = m_board->position[row][col].power;
                m_board->position[row][col].power = power - 1;

                if (m_board->position[row][col].power <= 0)
                {
                    Point p;
                    p.x = row;
                    p.y = col;
                    board_remove_card(m_board, p);
                }
            }
        }
    }
}

bool is_match_arrow(Board *m_board, Point m_attacker, Point m_defender)
{
    switch (get_case(m_defender)) {
    case 0:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | D | A | * |
         * |---|---|---|
         * | A | * | * |
         * |---|---|---|
         * | * | * | * |
         * ---------------
         */

        // Define as posições dos possíveis atacantes
        const Point attacks[2] = { {0,1}, {1,0} };

        // Define as direções possíveis das setas do defensor
        const int possible_directions[2] = {RIGHT, BOTTOM};

        // Armazenará as direções válidas
        int directions[2];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (attacks) / sizeof(attacks[0]);

        return defense_case(m_board, m_attacker, m_defender, attacks, possible_directions, directions, loop);
    }

    case 1:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | A | D | A |
         * |---|---|---|
         * | * | A | * |
         * |---|---|---|
         * | * | * | * |
         * ---------------
         * */

        // Define as posições dos possíveis atacantes
        const Point attacks[3] = { {0,0}, {0,2}, {1,1} };

        // Define as direções possíveis das setas do defensor
        const int possible_directions[3] = {LEFT, RIGHT, BOTTOM};

        // Armazenará as direções válidas
        int directions[3];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (attacks) / sizeof(attacks[0]);

        return defense_case(m_board, m_attacker, m_defender, attacks, possible_directions, directions, loop);
    }

    case 2:
     {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | A | D |
         * |---|---|---|
         * | * | * | A |
         * |---|---|---|
         * | * | * | * |
         * ---------------
         * */

        // Define as posições dos possíveis atacantes
        const Point attacks[2] = { {0,1}, {1,2} };

        // Define as direções possíveis das setas do defensor
        const int possible_directions[2] = {LEFT, BOTTOM};

        // Armazenará as direções válidas
        int directions[2];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (attacks) / sizeof(attacks[0]);

        return defense_case(m_board, m_attacker, m_defender, attacks, possible_directions, directions, loop);
    }

    case 3:
     {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | A | * | * |
         * |---|---|---|
         * | D | A | * |
         * |---|---|---|
         * | A | * | * |
         * ---------------
         * */

        // Define as posições dos possíveis atacantes
        const Point attacks[3] = { {0,0}, {1,1}, {2,0} };

        // Define as direções possíveis das setas do defensor
        const int possible_directions[3] = {TOP, RIGHT, BOTTOM};

        // Armazenará as direções válidas
        int directions[3];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (attacks) / sizeof(attacks[0]);

        return defense_case(m_board, m_attacker, m_defender, attacks, possible_directions, directions, loop);
    }

    case 4:
     {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | A | * |
         * |---|---|---|
         * | A | D | A |
         * |---|---|---|
         * | * | A | * |
         * ---------------
         * */

        // Define as posições dos possíveis atacantes
        const Point attacks[4] = { {0,1}, {1,0}, {1,2}, {2,1} };

        // Define as direções possíveis das setas do defensor
        const int possible_directions[4] = {TOP, LEFT, RIGHT, BOTTOM};

        // Armazenará as direções válidas
        int directions[4];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (attacks) / sizeof(attacks[0]);

        return defense_case(m_board, m_attacker, m_defender, attacks, possible_directions, directions, loop);
    }

    case 5:
     {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | * | A |
         * |---|---|---|
         * | * | A | D |
         * |---|---|---|
         * | * | * | A |
         * ---------------
         * */

        // Define as posições dos possíveis atacantes
        const Point attacks[3] = { {0,2}, {1,1}, {2,2} };

        // Define as direções possíveis das setas do defensor
        const int possible_directions[3] = {TOP, LEFT, BOTTOM};

        // Armazenará as direções válidas
        int directions[3];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (attacks) / sizeof(attacks[0]);

        return defense_case(m_board, m_attacker, m_defender, attacks, possible_directions, directions, loop);
    }

    case 6:
     {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | * | * |
         * |---|---|---|
         * | A | * | * |
         * |---|---|---|
         * | D | A | * |
         * ---------------
         * */

        // Define as posições dos possíveis atacantes
        const Point attacks[2] = { {1,0}, {2,1} };

        // Define as direções possíveis das setas do defensor
        const int possible_directions[2] = {TOP, RIGHT};

        // Armazenará as direções válidas
        int directions[2];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (attacks) / sizeof(attacks[0]);

        return defense_case(m_board, m_attacker, m_defender, attacks, possible_directions, directions, loop);
    }

    case 7:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | * | * |
         * |---|---|---|
         * | * | A | * |
         * |---|---|---|
         * | A | D | A |
         * ---------------
         * */

        // Define as posições dos possíveis atacantes
        const Point attacks[3] = { {1,1}, {2,0}, {2,2} };

        // Define as direções possíveis das setas do defensor
        const int possible_directions[3] = {TOP, LEFT, RIGHT};

        // Armazenará as direções válidas
        int directions[3];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (attacks) / sizeof(attacks[0]);

        return defense_case(m_board, m_attacker, m_defender, attacks, possible_directions, directions, loop);
    }

    case 8:
    {
        /*
         * A = Ataque
         * D = Defesa
         * ---------------
         * | * | * | * |
         * |---|---|---|
         * | * | * | A |
         * |---|---|---|
         * | * | A | D |
         * ---------------
         * */

        // Define as posições dos possíveis atacantes
        const Point attacks[2] = { {1,2}, {2,1} };

        // Define as direções possíveis das setas do defensor
        const int possible_directions[2] = {TOP, LEFT};

        // Armazenará as direções válidas
        int directions[2];

        // Define o tamanho dos array para fazer os loops necessários
        const size_t loop = sizeof (attacks) / sizeof(attacks[0]);

        return defense_case(m_board, m_attacker, m_defender, attacks, possible_directions, directions, loop);
    }

    default:
        puts("ERRO! -> (has_defense_arrow_to_attacker)");
        break;
    }

    return false;
}

bool defense_case(Board *m_board, const Point m_attacker, const Point m_defender,
                  const Point *m_attacks, const int *m_possible_directions, int *m_directions, const size_t m_loop)
{
    get_direction(m_board, m_attacker, m_possible_directions, m_directions, m_loop);

    for (int i = 0; i < (int)(m_loop); ++i)
    {
        if ((m_attacker.x == m_attacks[i].x && m_attacker.y == m_attacks[i].y) )
        {
            if (m_board->position[m_defender.x][m_defender.y].arrows[m_possible_directions[i]])
            {
                return true;
            }
        }
    }

    return false;
}

void score_case_default(Board *m_board, const Point m_attacker, const Point m_defender)
{
    int attacker_power = m_board->position[m_attacker.x][m_attacker.y].power;
    int defender_power = m_board->position[m_defender.x][m_defender.y].power;

    m_board->position[m_defender.x][m_defender.y].power = defender_power - attacker_power;
}

void score_case1(Board *m_board, const Point m_attacker, const Point m_defender)
{
    int attacker_power = m_board->position[m_attacker.x][m_attacker.y].power;
    int defender_power = m_board->position[m_defender.x][m_defender.y].power;

    // Calcula a diferença entre as cartas
    int difference = abs(attacker_power - defender_power);

    // Subtrai da carta atacada o valar da diferença entre as duas
    m_board->position[m_defender.x][m_defender.y].power = defender_power - difference;
}

void score_case2(Board *m_board, const Point m_defender)
{
    int defender_power = m_board->position[m_defender.x][m_defender.y].power;

    // Subtrai 1 ponto de poder da carta defesa
    m_board->position[m_defender.x][m_defender.y].power = defender_power - 1;
}

void elemental_damage(Board *m_board, const Point m_attacker, const Point m_defender)
{
    // TODO
    /*
     * Dano Elemental
     * Ao atacar outra, após o cálculo de dano de ataque, se uma carta possui um dos três elementos, ela pode
     * mudar o status elemental da carta atacada, alterando o comportamento desta durante a partida:
     *
     * i. Fogo
     * Ao ser atacada por uma carta do elemento “fogo”, uma carta tem seu status alterado para “incendiada”.
     *
     * Quando atacadas, cartas “incendiadas” causam 1 ponto de dano aos seus atacantes imediatamente.
     *
     * ii. Gelo
     * Ao ser atacada por uma carta do elemento “gelo”, uma carta tem seu status alterado para “congelada”.
     * Enquanto estiver “congelada”, uma carta tem suas orientações de ataque ignoradas no cálculo de dano.
     * Isso significa que elas SEMPRE sofrerão o dano completo ao serem atacadas (mesmo que possuam orientações
     * de ataque opostas às das cartas atacantes).
     *
     * iii. Veneno
     * Ao ser atacada por uma carta do elemento “veneno”, uma carta tem seu status alterado para “envenenada”.
     * Ao final de um turno, todas as cartas “envenenadas” no tabuleiro perdem 1 ponto de poder.
     * Observação: quando uma carta “congelada” é atacada por uma carta do elemento “fogo” ela não tem seu status
     * alterado para “incendiada” e vice-versa. Nesses casos, a carta retorna ao seus status neutro (isto é, normal).
    */

    int state = (int)(m_board->position[m_attacker.x][m_attacker.y].element);

    switch (state) {
    case FIRE:
    {
        if(m_board->position[m_defender.x][m_defender.y].status == FROZEN)
        {
            m_board->position[m_defender.x][m_defender.y].status = NO_STATUS;
        }
        else
        {
            m_board->position[m_defender.x][m_defender.y].status = BURNED;
        }
    }
        break;
    case ICE:
    {
        if(m_board->position[m_defender.x][m_defender.y].status == BURNED)
        {
            m_board->position[m_defender.x][m_defender.y].status = NO_STATUS;
        }
        else
        {
            m_board->position[m_defender.x][m_defender.y].status = FROZEN;
        }

        break;
    }
    case POISON:
    {
        m_board->position[m_defender.x][m_defender.y].status = POISONED;

        break;
    }

    }
}

void render(const Board *m_board, Pack *m_pack,
            const Player *m_player1, const Player *m_player2)
{
    // Imprime o tabuleiro
    board_render(m_board, m_render_mode);

    // Imprime a quantidade de cartas restantes no baralho
    printf("           Cartas no deck: [%zu]\n", pack_size());

    // Imprime a próxima carta
    pack_print_next_card(m_pack, m_render_mode);

    // Imprime placar
    print_score(m_player1, m_player2);
}


void print_score(const Player *m_player1, const Player *m_player2)
{
    printf("%s: %d               VS             %s: %d\n\n",
           m_player1->name, m_player1->score, m_player2->name, m_player2->score);
}

void print_scoreboard(const Player *m_player1, const Player *m_player2)
{
    limparTela();
    deckmaster_logo(m_render_mode);

    char color_logo[8] = "";
    char color_equal[8] = "";
    char color_winner[8] = "";
    char color_reset[8] = "";

    if (m_render_mode == COLOR_MODE)
    {
        memcpy(color_logo, "\x1b[96m", sizeof(color_logo));
        memcpy(color_equal, "\x1b[93m", sizeof(color_equal));
        memcpy(color_winner, "\x1b[32m", sizeof(color_winner));
        memcpy(color_reset, "\x1b[0m", sizeof(color_reset));
    }

    printf("\n\n %s", color_logo);
    puts("                   ___  __    _    __   _   ___    ____ __ _  __  _   __     ");
    puts("                  / o |/ /  .' \\ ,'_/ .' \\ / o |  / __// // |/ /.' \\ / /  ");
    puts("                 / _,'/ /_ / o // /_ / o //  ,'  / _/ / // || // o // /_     ");
    puts("                /_/  /___//_n_/ |__//_n_//_/`_\\ /_/  /_//_/|_//_n_//___/    ");
    printf("\n\n %s", color_reset);

    printf("                      %s: %d               VS             %s: %d\n\n",
           m_player1->name, m_player1->score, m_player2->name, m_player2->score);

    puts("\n\n");

    if (m_player1->score == m_player2->score)
    {
        printf("%s                        ************* EMPATE ************* \n\n\n\n\n\n %s", color_equal, color_reset);
    }
    else
    {
        char winner_player[100] = "";
        memcpy(winner_player, (m_player1->score > m_player2->score) ? m_player1->name : m_player2->name, sizeof(winner_player));

        printf("%s              ************* PARABÉNS %s VOCÊ VENCEU!!! ************* \n\n\n\n\n\n %s", color_winner ,
               winner_player, color_reset);
    }

    printf("Digite qualquer character para continuar: ");
    char m_exit[1];
    scanf("%s", m_exit);
}

void deckmaster_logo(unsigned int m_render)
{
    char color_logo[8] = "";
    char color_reset[8] = "";

    if (m_render == COLOR_MODE)
    {
        memcpy(color_logo, "\x1b[96m", sizeof(color_logo));
        memcpy(color_reset, "\x1b[0m", sizeof(color_reset));
    }

    printf("\n%s", color_logo);
    puts("******************************************************************************************");
    printf(" _____    ______    _____   _  __  __  __               _____   _______   ______   _____  \n");
    printf("|  __ \\  |  ____|  / ____| | |/ / |  \\/  |     /\\      / ____| |__   __| |  ____| |  __ \\ \n");
    printf("| |  | | | |__    | |      | ' /  | \\  / |    /  \\    | (___      | |    | |__    | |__) |\n");
    printf("| |  | | |  __|   | |      |  <   | |\\/| |   / /\\ \\    \\___ \\     | |    |  __|   |  _  / \n");
    printf("| |__| | | |____  | |____  | . \\  | |  | |  / ____ \\   ____) |    | |    | |____  | | \\ \\ \n");
    printf("|_____/  |______|  \\_____| |_|\\_\\ |_|  |_| /_/    \\_\\ |_____/     |_|    |______| |_|  \\_\\\n");
    puts("\n******************************************************************************************");
    printf("\n%s", color_reset);
}
