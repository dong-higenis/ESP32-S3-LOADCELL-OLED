Hx711_calibration을 실행하고 무게를 알고있는 사물의 측정값을 기록한다.
측정값과 실제 무게를 나누면 캘리브레이션 값이 된다. 
Calibration factor = reading / weight
이 값을 hx711 예제에서 scale.set_scale() 함수의 인자로 전달한다.


