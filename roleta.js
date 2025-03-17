document.addEventListener("DOMContentLoaded", () => {
    const form = document.getElementById('form-roleta');
    const resultadoDiv = document.getElementById('resultado');

    form.addEventListener('submit', function(event) {
        event.preventDefault();

        // Pegando os dados do formulário
        const numOpcoes = parseInt(document.getElementById('opcoes').value);
        const opcoesText = document.getElementById('opcao').value;
        
        // Dividindo as opções do texto e garantindo que haja o número correto de opções
        const opcoes = opcoesText.split('\n').map(opcao => opcao.trim()).filter(opcao => opcao.length > 0);
        
        if (opcoes.length !== numOpcoes) {
            exibirResultado("O número de opções não corresponde ao valor informado.");
            return;
        }

        // Função para girar a roleta
        const sorteado = Math.floor(Math.random() * numOpcoes);
        exibirResultado(`A roleta parou em: ${opcoes[sorteado]}`);
    });

    function exibirResultado(mensagem) {
        resultadoDiv.innerHTML = `<p>${mensagem}</p>`;
    }
});
