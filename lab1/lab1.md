<script type="text/javascript"
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.0/MathJax.js?config=TeX-AMS_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    tex2jax: {
      inlineMath: [['$','$'], ['\\(','\\)']],
      processEscapes: true},
      jax: ["input/TeX","input/MathML","input/AsciiMath","output/CommonHTML"],
      extensions: ["tex2jax.js","mml2jax.js","asciimath2jax.js","MathMenu.js","MathZoom.js","AssistiveMML.js", "[Contrib]/a11y/accessibility-menu.js"],
      TeX: {
      extensions: ["AMSmath.js","AMSsymbols.js","noErrors.js","noUndefined.js"],
      equationNumbers: {
      autoNumber: "AMS"
      }
    }
  });
</script>

<div align="center">

# Wprowadzenie do Sztucznej Intelignecji - lab 1

Jakub Musiał

<div align="left">

<br />

## 1. Wprowadznie

* Zadanie - rozwiązać problem układanki [15 Puzzle](https://en.wikipedia.org/wiki/15_puzzle)

* Rozwiązanie - algorytm A* z uzyciem heurystyk:
  * Manhattan distance
  * Inversion distance

* Implementacja: C++

<br />
<br />

## 2. Czas rozwiązania

Poniższy wykres przedstawia czasy rozwiązania układanki względem długości rozwiązania oraz średnie czasy rozwiązań dla zadanych heurystyk. Można zauważyć, że haurystyka _manhattan distance_ zapewnia znacząco krótszy czas rozwiązania niż _inversion distance_.

<div align="center">

![](plots/time.png)

<div align="left">

<br />
<br />

## 3. Liczba odwiedzonych stanów

Czas wykonania programu jest w pewien sposób proporcjonalny do liczby odwiedzonych stanów. Widać to także na poniższym wykresie. Jednak w tej kategorii przewaga _manhattan distance_ nad _inversion distance_ przestaje być zauważalna.

<div align="center">

![](plots/visited.png)

<div align="left">

Warto zwrócić uwagę na to, iż średnia liczba odwiedzonych stanów jest rzędu $< 10^7$, co jest zdecydowanie mniejszą wartością niż liczba wszytskich poprawnych permutacji, która wynosi $\frac{15!}{2} \approx 6 \cdot 10^{11}$.
