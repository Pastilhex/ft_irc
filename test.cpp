/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 19:23:23 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/28 19:23:23 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>

struct Quiz
{
	std::string titulo;
	std::vector<std::string> options;
	int respostaCorreta; // Índice da resposta correta (começando de 0)
};

void executarQuiz(const std::vector<Quiz> &quiz)
{
	int pontuacao = 0;

	for (const auto &Quiz : quiz)
	{
		std::cout << Quiz.titulo << "\n";

		for (size_t i = 0; i < Quiz.options.size(); ++i)
		{
			std::cout << i + 1 << ". " << Quiz.options[i] << "\n";
		}

		int resposta;
		std::cout << "Digite o número da resposta correta: ";
		std::cin >> resposta;

		if (resposta - 1 == Quiz.respostaCorreta)
		{
			std::cout << "Resposta correta!\n\n";
			pontuacao++;
		}
		else
		{
			std::cout << "Resposta errada. A resposta correta era: "
					  << Quiz.options[Quiz.respostaCorreta] << "\n\n";
		}
	}

	std::cout << "Você acertou " << pontuacao << " de " << quiz.size() << " perguntas.\n";
}

int main()
{
	// Crie um vetor com Quizs e respostas
	std::vector<Quiz> quiz = {
		{
			"Qual é o elemento químico com o símbolo 'O'?",
			{"Ouro", "Oxigênio", "Ósmio", "Oxigênio"},
			1 // Índice da resposta correta (Oxigênio)
		},
		{
			"Quem escreveu 'Dom Casmurro'?",
			{"Machado de Assis", "José de Alencar", "Eça de Queirós", "Guimarães Rosa"},
			0 // Índice da resposta correta (Machado de Assis)
		}};

	// Executa o quiz
	executarQuiz(quiz);

	return 0;
}
