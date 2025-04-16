clear; clc;

% --- Seri Port Ayarları ---
portName = "COM10"; % <- kendi COM port'unu buraya yaz
baudRate = 115200;
serialObj = serialport(portName, baudRate);
flush(serialObj);

% --- Gelişmiş Ayarlar ---
windowSize = 1000;  % <- Burayı artırdık
dataRoll = nan(1, windowSize);
dataPitch = nan(1, windowSize);
dataYaw = nan(1, windowSize);
sampleIndex = 0;

% --- Plot Ayarları ---
figure;
%h1 = plot(nan, nan, 'r', 'DisplayName', 'Roll'); hold on;
%h2 = plot(nan, nan, 'g', 'DisplayName', 'Pitch');
%h3 = plot(nan, nan, 'b', 'DisplayName', 'Yaw');
h1 = plot(nan, nan, 'r', 'LineWidth', 2, 'DisplayName', 'Roll'); hold on;
h2 = plot(nan, nan, 'g', 'LineWidth', 2, 'DisplayName', 'Pitch');
h3 = plot(nan, nan, 'b', 'LineWidth', 2, 'DisplayName', 'Yaw');


legend;
ylim([-180 180]); grid on;
xlabel('Zaman (örnek sayısı)'); ylabel('Açı (derece)');
title('Gerçek Zamanlı Euler Açıları (Roll, Pitch, Yaw)');

% --- Ana Döngü ---
while true
    try
        line = readline(serialObj);
        values = sscanf(line, '%f,%f,%f');
        
        if numel(values) == 3
            valuesDeg = rad2deg(values);
            sampleIndex = sampleIndex + 1;
            
            % Kaydırmalı buffer (scrolling plot)
            if sampleIndex > windowSize
                dataRoll = [dataRoll(2:end), valuesDeg(1)];
                dataPitch = [dataPitch(2:end), valuesDeg(2)];
                dataYaw = [dataYaw(2:end), valuesDeg(3)];
                xVals = sampleIndex - windowSize + 1 : sampleIndex;
            else
                dataRoll(sampleIndex) = valuesDeg(1);
                dataPitch(sampleIndex) = valuesDeg(2);
                dataYaw(sampleIndex) = valuesDeg(3);
                xVals = 1:sampleIndex;
            end

            % Güncelle
            set(h1, 'XData', xVals, 'YData', dataRoll(~isnan(dataRoll)));
            set(h2, 'XData', xVals, 'YData', dataPitch(~isnan(dataPitch)));
            set(h3, 'XData', xVals, 'YData', dataYaw(~isnan(dataYaw)));
            xlim([max(1, sampleIndex - windowSize + 1), sampleIndex]);
            drawnow limitrate;
        end
    catch ME
        disp("Hata: " + ME.message);
    end
end
