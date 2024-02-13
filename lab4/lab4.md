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

# Wprowadzenie do Sztucznej Intelignecji - lab 4

Jakub Musiał

<div align="left">

<br />

## Zadanie 1

### 1. Wprowadznie

* Zadanie: wytrenowanie modelu lasu decyzyjnego rozpoznającego cyfry ze zbioru [MNIST](https://en.wikipedia.org/wiki/MNIST_database)

* Implementacja: Python

<br />

### 2. Trenowanie modelu

Model był trenowany używając parametrów:

* Liczba drzew $= 30$

* Metryka - `accuracy`

<br />

### 3. Wyniki

Skuteczność modelu na zbiorze testowym [MNIST](https://en.wikipedia.org/wiki/MNIST_database) - $0.9629$

<br />
<br />
<br />

## Zadanie 2

### 1. Wprowadznie

* Zadanie: implementacja modelu DBSCAN rozpoznającego cyfry ze zbioru [MNIST](https://en.wikipedia.org/wiki/MNIST_database)

* Implementacja: Python

<br />

### 2. Trenowanie modelu

Dane do treningu były przetworzone modelem PCA (Principle Component Analysis) - redukcja wymiaru polegająca na grupowainu po minimalnych i maksymalnych koordynatach najbardziej wysuniętych zaznaczonych punktów

Model był trenowany używając parametrów:

* $EPS = 3$ 

* $minPts = 1000$ 

> EPS - Promień określonych otoczeń. Jeśli odległość między dwoma punktami jest mniejsza lub równa ESP, zostaną one uznane za sąsiadów.
>
> minPts - Minimalna liczba punktów w danym otoczeniu, aby utworzyć klastry. 

<br />

### 3. Wyniki

* Liczba klastrów: $24$

* Skuteczność: $0.68$

* Zaszumienie: $0.07$

Poniże wykresy przedstawiaja wyniki modelu

<div align="center">

![](plots/pca.png)

<br />

![](plots/pca_to_dbscan.png)

<br />

![](plots/result.png)

<div align="left">
