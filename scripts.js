document.addEventListener("DOMContentLoaded", () => {
    const form = document.getElementById('form-simulador');
    const resultadoDiv = document.getElementById('resultado');

    let saldo = 0.0;
    let nomeTitular = '';
    let numeroConta = 0;

    function exibirResultado(mensagem) {
        resultadoDiv.innerHTML = `<p>${mensagem}</p>`;
    }

    form.addEventListener('submit', function(event) {
        event.preventDefault();

        nomeTitular = document.getElementById('nome').value;
        numeroConta = document.getElementById('numeroConta').value;
        const acao = document.getElementById('acao').value;
        const valor = parseFloat(document.getElementById('valor').value) || 0;

        // Lógica do simulador de banco
        if (acao === "1") {
            exibirResultado(`Saldo atual de ${nomeTitular} (Conta #${numeroConta}): R$ ${saldo.toFixed(2)}`);
        } else if (acao === "2") {
            if (valor > 0) {
                saldo += valor;
                exibirResultado(`Depósito de R$ ${valor.toFixed(2)} realizado com sucesso! Saldo atual: R$ ${saldo.toFixed(2)}`);
            } else {
                exibirResultado("Valor inválido para depósito!");
            }
        } else if (acao === "3") {
            if (valor > 0 && valor <= saldo) {
                saldo -= valor;
                exibirResultado(`Retirada de R$ ${valor.toFixed(2)} realizada com sucesso! Saldo atual: R$ ${saldo.toFixed(2)}`);
            } else {
                exibirResultado("Valor inválido ou saldo insuficiente para a retirada!");
            }
        }
    });
});
