clear; close all; clc;

function visualization()
    % Загрузка данных
    types = {'random', 'reverse', 'almost'};
    type_names = {'случайные', 'обратные', 'почти отсорт.'};
    thresholds = [5, 10, 20, 30, 50];

    fprintf('Создание графиков...\n');

    % Создание графиков для каждого типа массива
    for i = 1:length(types)
        type = types{i};
        type_name = type_names{i};
        fprintf('Построение графиков для %s массивов...\n', type_name);
        plotResultsForType(type, type_name, thresholds);
    end

    % Сравнительный анализ порогов
    compareThresholds(types, type_names, thresholds);

    fprintf('Визуализация в Octave завершена!\n');
end

function plotResultsForType(type, type_name, thresholds)
    % Построение графиков для конкретного типа массива

    fprintf('Загрузка данных для %s массивов...\n', type_name);

    % Загрузка данных стандартного Merge Sort
    mergeData = csvread(sprintf('merge_%s.csv', type), 1, 0);
    sizes = mergeData(:, 1);
    mergeTimes = mergeData(:, 2);

    % Создание figure
    figure('position', [100, 100, 1200, 800]);

    % График 1: Сравнение производительности
    subplot(2, 1, 1);
    plot(sizes, mergeTimes, 'b-', 'linewidth', 2);
    hold on;

    colors = ['r', 'g', 'm', 'c', 'k'];
    legend_entries = {'Стандартный Merge Sort'};

    % Загрузка и построение гибридных алгоритмов
    for j = 1:length(thresholds)
        threshold = thresholds(j);
        hybridData = csvread(sprintf('hybrid_%s_th%d.csv', type, threshold), 1, 0);
        hybridTimes = hybridData(:, 2);

        plot(sizes, hybridTimes, [colors(j) '--'], 'linewidth', 1.5);
        legend_entries{end+1} = sprintf('Гибридный (порог=%d)', threshold);
    end

    title(sprintf('Сравнение производительности - %s массивы', type_name), 'fontsize', 14);
    xlabel('Размер массива', 'fontsize', 12);
    ylabel('Время (микросекунды)', 'fontsize', 12);
    legend(legend_entries, 'location', 'northwest');
    grid on;

    % График 2: Отношение времени
    subplot(2, 1, 2);

    % Используем threshold=20 для сравнения
    bestThreshold = 20;
    bestHybridData = csvread(sprintf('hybrid_%s_th%d.csv', type, bestThreshold), 1, 0);
    bestHybridTimes = bestHybridData(:, 2);

    ratio = bestHybridTimes ./ mergeTimes;
    plot(sizes, ratio, 'k-', 'linewidth', 2);
    hold on;
    plot(sizes, ones(size(sizes)), 'r--', 'linewidth', 1);

    title(sprintf('Отношение скорости: Гибридный(порог=%d) / Стандартный - %s массивы', ...
          bestThreshold, type_name), 'fontsize', 14);
    xlabel('Размер массива', 'fontsize', 12);
    ylabel('Отношение времени', 'fontsize', 12);
    legend('Отношение Гибридный/Стандартный', 'Базовый уровень (отношение=1)', 'location', 'best');
    grid on;

    % Сохранение графика
    print(sprintf('comparison_%s.png', type), '-dpng', '-r300');

    drawnow;
    pause(1);
end

function compareThresholds(types, type_names, thresholds)
    % Сравнительный анализ эффективности порогов

    figure('position', [100, 100, 1500, 600]);

    for i = 1:length(types)
        type = types{i};
        type_name = type_names{i};

        % Загрузка данных стандартного алгоритма
        mergeData = csvread(sprintf('merge_%s.csv', type), 1, 0);
        mergeTimes = mergeData(:, 2);

        subplot(1, 3, i);

        % Расчет улучшения для каждого порога
        improvements = zeros(length(thresholds), 1);

        for j = 1:length(thresholds)
            threshold = thresholds(j);
            hybridData = csvread(sprintf('hybrid_%s_th%d.csv', type, threshold), 1, 0);
            hybridTimes = hybridData(:, 2);

            % Среднее улучшение по всем размерам
            ratio = mergeTimes ./ hybridTimes;
            improvements(j) = mean(ratio(isfinite(ratio))); % Убираем NaN/Inf
        end

        % Построение графика улучшений
        bar(improvements, 'facecolor', [0.2 0.6 0.8]);
        set(gca, 'xticklabel', arrayfun(@num2str, thresholds, 'uniformoutput', false));
        title(sprintf('Среднее улучшение - %s массивы', type_name), 'fontsize', 12);
        xlabel('Порог переключения', 'fontsize', 10);
        ylabel('Коэффициент ускорения (Merge/Hybrid)', 'fontsize', 10);
        grid on;

        % Добавление числовых значений на столбцы
        for j = 1:length(thresholds)
            text(j, improvements(j), sprintf('%.3f', improvements(j)), ...
                'horizontalalignment', 'center', 'verticalalignment', 'bottom', ...
                'fontsize', 8);
        end
    end

    print('threshold_analysis.png', '-dpng', '-r300');
    fprintf('Сохранено: threshold_analysis.png\n');

    drawnow;
    pause(1);
end

visualization();
