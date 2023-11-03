# Open Automotive Simulator (Unreal Engine Modification)
<br>
<div align="center">
<img style="center" src="https://i.imgur.com/RFZr7SQ.png" alt="drawing" width="250"/>
</div>
<br>


O Open Automotive Simulator (OAS) é um projeto que visa desenvolver simulador automotivo de uso gratuito, de código aberto e de alta fidelidade com relação ao comportamento dinâmico real de um veículo. O OAS utiliza de modificações na [Unreal Engine](https://github.com/EpicGames/UnrealEngine) para satisfazer os requisitos do projeto e neste repositório é armazenado o código fonte da Engine com as modificações necessárias.






Iniciar e executar :computer:
----------------------

Os passos a seguir explicam como clonar seu próprio fork privado, compilar e executar o editor por si mesmo:

### Windows

1. Instale o **[GitHub Desktop para Windows](https://desktop.github.com/)** e, em seguida, **[faça um fork e clone nosso repositório](https://guides.github.com/activities/forking/)**.

    Quando o GitHub Desktop terminar de criar o fork, ele perguntará: **Como você planeja usar este fork?**. Selecione **Para meus próprios fins**. Isso ajudará a garantir que quaisquer alterações que você fizer permaneçam locais em seu repositório e evitem a criação de solicitações de pull não intencionais. Ainda assim, você poderá fazer solicitações de pull se desejar enviar modificações que você fizer em seu fork de volta para nosso repositório.

    Outras opções:

    - Para usar o Git a partir da linha de comando, consulte os artigos [Configurando o Git](https://help.github.com/articles/set-up-git/) e [Fazendo um Fork de um Repositório](https://help.github.com/articles/fork-a-repo/).

    - Se preferir não usar o Git, você pode obter o código-fonte com o botão **Download ZIP** à direita. Observe que o utilitário zip integrado ao Windows marca o conteúdo de arquivos .zip baixados da Internet como não seguro para execução. Portanto, clique com o botão direito no arquivo .zip, selecione **Propriedades...** e **Desbloquear** antes de descompactá-lo.

1. Instale o **Visual Studio 2022** ou **Visual Studio 2019**.

    Todas as edições desktop do Visual Studio 2022 e 2019 podem compilar o UE5, incluindo o [Visual Studio Community](https://www.visualstudio.com/products/visual-studio-community-vs), que é gratuito para pequenas equipes e desenvolvedores individuais.

    Para instalar os componentes corretos para o desenvolvimento do UE5, consulte [Configurando o Visual Studio](https://docs.unrealengine.com/en-US/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine/).

1. Abra sua pasta de origem no Windows Explorer e execute **Setup.bat**. Isso fará o download de conteúdo binário para o motor, instalará pré-requisitos e configurará associações de arquivo do Unreal.

    No Windows 8, um aviso do SmartScreen pode aparecer. Clique em **Mais informações** e, em seguida, **Executar de qualquer maneira** para continuar.

    Um download limpo dos binários do motor atualmente tem um tamanho de 20-21 GiB, o que pode levar algum tempo para ser concluído. As execuções subsequentes serão muito mais rápidas, pois elas apenas baixam conteúdo novo e atualizado.

1. Execute **GenerateProjectFiles.bat** para criar os arquivos de projeto para o motor. Isso deve levar menos de um minuto para ser concluído.

1. Carregue o projeto no Visual Studio clicando duas vezes no novo arquivo **UE5.sln**.

1. Defina a configuração da solução como **Desenvolvedor de Editor** e a plataforma da solução como **Win64**, em seguida, clique com o botão direito no destino **UE5** e selecione **Compilar**. Isso pode levar de 10 a 40 minutos para ser concluído, dependendo das especificações do seu sistema.

1. Após a conclusão da compilação, você pode executar o editor a partir do Visual Studio, definindo o projeto de inicialização como **UE5** e pressionando **F5** para iniciar a depuração.
---------------

<br>
<div align="center">
<img style="center" src="https://fga.unb.br/articles/0000/2640/unb-fga-extenso.jpg" alt="drawing" width="800"/>
</div>
<br>
