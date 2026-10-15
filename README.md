<h1 align="center"> 🐂 BoviCount </h1>

<p align="center">
  <i>Software desktop de gestão rural para inventário automatizado de gado via Inteligência Artificial.</i>
</p>

O **BoviCount** é um software desktop de gestão rural desenvolvido para automatizar o inventário de gado de maneira não-intrusiva, eliminando a necessidade do estressante e custoso manejo manual dos animais até o curral. Utilizando inteligência artificial e visão computacional por meio do motor OpenCV e de um modelo YOLO pré-treinado, o sistema processa imagens aéreas dos pastos para identificar, demarcar com caixas delimitadoras (*bounding boxes*) e contar automaticamente os animais. O objetivo central da aplicação é garantir a precisão da contagem, reduzir altos custos logísticos e preservar o bem-estar do rebanho, mitigando perdas financeiras decorrentes de furtos ou machucados na pecuária extensiva.

Operando de forma totalmente *offline* para atender à realidade de baixa conectividade nas zonas rurais, o BoviCount armazena seus registros localmente utilizando um banco de dados embutido SQLite. Além do robusto módulo de contagem por IA, a aplicação fornece um ambiente completo de gestão (CRUD) para o controle administrativo de funcionários, pastos e capacidades de lotes. Essa arquitetura permite cruzar os dados de quem operou o sistema com a data, o pasto de origem e a mídia processada, gerando um histórico seguro, exportável em CSV e auditável para os gestores da propriedade.