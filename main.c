/*
 * Автор: Трофимовский Артём Анатольевич, группа бОТИ-261
 *
 * Курсовой проект "Реализация программы контроля облачной инфраструктуры"
 * Вариант 27. КТ1 - пять тестовых значений.
 * Считаю Q = (X[i] - X[i-k]) / X[i-k] для PWR, LAT, QRY
 * и проверяю правило "два из трех".
 */

#include <stdio.h>
#include <locale.h>

#define N 5		/* сколько отсчетов */
#define K 3		/* период сравнения k */
#define LIMIT 2.0f	/* порог L */
#define MIN_ALARM 2	/* сколько компонентов в тревоге нужно для инцидента */

float calc_q(float x_now, float x_old);
int is_alarm(float q);

int main()
{
	/* данные из карточки, разложил по времени t = 1..5, ZZZ выкинул */
	float pwr[N] = { 45.5f, 46.0f, 46.5f, 47.0f, 52.9f };
	float lat[N] = { 55.6f, 53.6f, 57.0f, 55.0f, 53.0f };
	float qry[N] = { 33.9f, 33.1f, 32.4f, 31.6f, 30.9f };
	float q_pwr, q_lat, q_qry;	/* Q для каждого компонента */
	int alarms;			/* сколько компонентов в тревоге на отсчете */
	int incidents = 0;		/* сколько всего инцидентов */
	int i;

	setlocale(LC_ALL, "RUS");

	printf("Здравствуйте! Это программа контроля облачной инфраструктуры (вариант 27).\n");
	printf("Она считает, насколько изменились показатели PWR, LAT, QRY\n");
	printf("за k отсчетов, и ищет инциденты: когда в тревоге 2 или 3 компонента.\n");
	printf("k = %d, L = %.1f\n\n", K, LIMIT);

	printf("отсчеты с 1 по %d пропускаю, для них нет X[i-k]\n\n", K);

	/* начинаю с i = K, потому что раньше не с чем сравнивать */
	for (i = K; i < N; i++) {
		q_pwr = calc_q(pwr[i], pwr[i - K]);
		q_lat = calc_q(lat[i], lat[i - K]);
		q_qry = calc_q(qry[i], qry[i - K]);

		alarms = 0;
		printf("t = %d\n", i + 1);

		printf("  PWR: (%.1f - %.1f) / %.1f = %.4f - ", pwr[i], pwr[i - K], pwr[i - K], q_pwr);
		if (is_alarm(q_pwr) == 1) {
			printf("ТРЕВОГА\n");
			alarms++;
		} else {
			printf("НОРМА\n");
		}

		printf("  LAT: (%.1f - %.1f) / %.1f = %.4f - ", lat[i], lat[i - K], lat[i - K], q_lat);
		if (is_alarm(q_lat) == 1) {
			printf("ТРЕВОГА\n");
			alarms++;
		} else {
			printf("НОРМА\n");
		}

		printf("  QRY: (%.1f - %.1f) / %.1f = %.4f - ", qry[i], qry[i - K], qry[i - K], q_qry);
		if (is_alarm(q_qry) == 1) {
			printf("ТРЕВОГА\n");
			alarms++;
		} else {
			printf("НОРМА\n");
		}

		/* правило "два из трех" */
		if (alarms >= MIN_ALARM) {
			printf("  в тревоге: %d - ИНЦИДЕНТ\n\n", alarms);
			incidents++;
		} else {
			printf("  в тревоге: %d - инцидента нет\n\n", alarms);
		}
	}

	printf("Всего инцидентов: %d\n", incidents);
	printf("\nНажмите Enter, чтобы выйти");
	getchar();
	return 0;
}

/*
 * считает Q = (x_now - x_old) / x_old
 * x_now - текущее значение X[i], x_old - значение k отсчетов назад X[i-k]
 * возвращает Q
 */
float calc_q(float x_now, float x_old)
{
	return (x_now - x_old) / x_old;
}

/*
 * проверяет в тревоге компонент или нет
 * q - посчитанное Q компонента
 * возвращает 1 если тревога (|q| > L), 0 если норма
 */
int is_alarm(float q)
{
	if (q > LIMIT || q < -LIMIT)
		return 1;
	return 0;
}
