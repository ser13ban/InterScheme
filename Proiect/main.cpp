#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <windows.h>


struct blocStruct
{
	int culoare = 0;
	sf::RectangleShape bloc;
	sf::Text text_bloc;

	blocStruct* urmatorul = NULL;
	int urmatorulIndex = 0;
	int doua_posibilitati = 0;
	blocStruct* urmatorul2 = NULL;
	int urmatorul2Index = 0;
	std::string expresie_bloc_text;
	sf::Text expresie_bloc;
};

std::vector<std::string> textBlocuri;
std::vector<sf::RectangleShape> meniu;
std::vector<sf::RectangleShape> butoane;
std::vector<blocStruct*> blocuri;
std::vector<sf::Text> texts;
std::vector<sf::Color> culori;

sf::String expresieString;
sf::Text textEroare, textCorect;


int limitStart = 0, limitEnd = 0;

int VALORI[1000], startID = -1;
//*2
void ruleazaBlocuri()
{
	for (int i = 0; i < 'Z' - 'A'; i++)
		VALORI[i] = 0;
	blocStruct* blocC = blocuri[startID];
	while (blocC)
	{
		/// Daca blocul are culoarea 0 inseamna ca este butonul START
		if (blocC->culoare == 0) {
			std::cout << "INCEPUT" << '\n';
			blocC = blocC->urmatorul;
		}

		/// Daca blocul are culoarea 1 inseamna ca este butonul END
		if (blocC->culoare == 1) {
			std::cout << "SFARSIT" << '\n';
			return;
		}

		/// Daca blocul are culoarea 2 inseamna ca este butonul INPUT
		if (blocC->culoare == 2)
		{
			std::cout << "Introduceti valoarea variabilei " << char(blocC->expresie_bloc_text[0]) << ": ";
			std::cin >> VALORI[(blocC->expresie_bloc_text[0]) - 'A'];
			blocC = blocC->urmatorul;
		}

		/// Daca blocul are culoarea 3 inseamna ca este butonul OUTPUT
		if (blocC->culoare == 3)
		{
			std::cout << "Valoarea variabilei " << char(blocC->expresie_bloc_text[0]) << " este: ";
			std::cout << VALORI[(blocC->expresie_bloc_text[0]) - 'A'] << '\n';
			blocC = blocC->urmatorul;
		}

		/// Daca blocul are culoarea 4 inseamna ca este butonul ASSIGN
		if (blocC->culoare == 4)
		{
		    /// ex: A=B
			if (blocC->expresie_bloc_text.size() == 3)
			{
				VALORI[(blocC->expresie_bloc_text[0]) - 'A'] = VALORI[(blocC->expresie_bloc_text[2]) - 'A'];
				blocC = blocC->urmatorul;
			}
			else
			{
			    /// ex: A=A+B
				if (blocC->expresie_bloc_text[3] == '+')
				{
					VALORI[(blocC->expresie_bloc_text[0]) - 'A'] = VALORI[(blocC->expresie_bloc_text[2]) - 'A'] + VALORI[(blocC->expresie_bloc_text[4]) - 'A'];
					blocC = blocC->urmatorul;
				}

				/// ex: A=A-B
				if (blocC->expresie_bloc_text[3] == '-')
				{
					VALORI[(blocC->expresie_bloc_text[0]) - 'A'] = VALORI[(blocC->expresie_bloc_text[2]) - 'A'] - VALORI[(blocC->expresie_bloc_text[4]) - 'A'];
					blocC = blocC->urmatorul;
				}

				/// ex: A=A*B
				if (blocC->expresie_bloc_text[3] == '*')
				{
					VALORI[(blocC->expresie_bloc_text[0]) - 'A'] = VALORI[(blocC->expresie_bloc_text[2]) - 'A'] * VALORI[(blocC->expresie_bloc_text[4]) - 'A'];
					blocC = blocC->urmatorul;
				}

				/// ex: A=A/B
				if (blocC->expresie_bloc_text[3] == '/')
				{
					VALORI[(blocC->expresie_bloc_text[0]) - 'A'] = VALORI[(blocC->expresie_bloc_text[2]) - 'A'] / VALORI[(blocC->expresie_bloc_text[4]) - 'A'];
					blocC = blocC->urmatorul;
				}
			}
		}

		/// Daca blocul are culoarea 5 inseamna ca butonul este COMPARE
		if (blocC->culoare == 5)
		{
			if (blocC->expresie_bloc_text[1] == '<')
			{

			    /// ex: A<=B
				if (blocC->expresie_bloc_text[2] == '=')
				{
					if (VALORI[(blocC->expresie_bloc_text[0]) - 'A'] <= VALORI[(blocC->expresie_bloc_text[3]) - 'A'])
						blocC = blocC->urmatorul;
					else blocC = blocC->urmatorul2;
				}

				/// ex: A<B
				else
				{
					if (VALORI[(blocC->expresie_bloc_text[0]) - 'A'] < VALORI[(blocC->expresie_bloc_text[2]) - 'A'])
						blocC = blocC->urmatorul;
					else blocC = blocC->urmatorul2;
				}
			}
			else if (blocC->expresie_bloc_text[1] == '>')
			{
			    /// ex: A>=B
				if (blocC->expresie_bloc_text[2] == '=')
				{
					if (VALORI[(blocC->expresie_bloc_text[0]) - 'A'] >= VALORI[(blocC->expresie_bloc_text[3]) - 'A'])
						blocC = blocC->urmatorul;
					else blocC = blocC->urmatorul2;
				}
				else
				{
				    /// ex: A>B
					if (VALORI[(blocC->expresie_bloc_text[0]) - 'A'] > VALORI[(blocC->expresie_bloc_text[2]) - 'A'])
						blocC = blocC->urmatorul;
					else blocC = blocC->urmatorul2;
				}
			}
			else if (blocC->expresie_bloc_text[1] == '=')
			{
			    /// ex: A=B
				if (VALORI[(blocC->expresie_bloc_text[0]) - 'A'] == VALORI[(blocC->expresie_bloc_text[3]) - 'A'])
					blocC = blocC->urmatorul;
				else blocC = blocC->urmatorul2;
			}
		}
	}
}
//*2
void StergereBlocuri()
{
	blocuri.clear();
	limitStart = false;
	limitEnd = false;
}
//creez meniul*1
void initMenu()
{
    //fac grila din linii gri deschis
	sf::RectangleShape topBar(sf::Vector2f(800.f, 3.f));
	topBar.setPosition(sf::Vector2f(0.f, 0.f));
	topBar.setFillColor(sf::Color(128, 128, 128));

	sf::RectangleShape midBar(sf::Vector2f(3.f, 600.f));
	midBar.setPosition(sf::Vector2f(600.f, 480.f));
	midBar.setFillColor(sf::Color(128, 128, 128));

	sf::RectangleShape lowerBar(sf::Vector2f(800.f, 3.f));
	lowerBar.setPosition(sf::Vector2f(0.f, 597.f));
	lowerBar.setFillColor(sf::Color(128, 128, 128));

	sf::RectangleShape leftBar(sf::Vector2f(3.f, 600.f));
	leftBar.setPosition(sf::Vector2f(0.f, 0.f));
	leftBar.setFillColor(sf::Color(128, 128, 128));

	sf::RectangleShape rightBar(sf::Vector2f(3.f, 600.f));
	rightBar.setPosition(sf::Vector2f(797.f, 0.f));
	rightBar.setFillColor(sf::Color(128, 128, 128));
    //grilajul butoanelor din dreapta jos
	sf::RectangleShape clearButton(sf::Vector2f(194.f, 65.f));
	clearButton.setPosition(sf::Vector2f(603.f,480.f));
	clearButton.setFillColor(sf::Color(204, 0, 0));

	sf::RectangleShape expresieTopBar(sf::Vector2f(800.f, 3.f));
	expresieTopBar.setPosition(sf::Vector2f(0.f, 480.f));
	expresieTopBar.setFillColor(sf::Color(128, 128, 128));

	sf::RectangleShape expresieLowBar(sf::Vector2f(194.f, 3.f));
	expresieLowBar.setPosition(sf::Vector2f(603.f, 545.f));
	expresieLowBar.setFillColor(sf::Color(128, 128, 128));

	sf::RectangleShape runButton(sf::Vector2f(194.f, 49.f));
	runButton.setPosition(sf::Vector2f(603.f, 548.f));
	runButton.setFillColor(sf::Color(0,153,0));


	meniu.push_back(topBar);
	meniu.push_back(rightBar);
	meniu.push_back(leftBar);
    meniu.push_back(lowerBar);
	meniu.push_back(midBar);
	meniu.push_back(clearButton);
	meniu.push_back(expresieTopBar);
	meniu.push_back(expresieLowBar);
	meniu.push_back(runButton);


}
//creez butoanele*1
void initButoane()
{
    sf::RectangleShape start(sf::Vector2f(170.0f, 50.0f));
	start.setOrigin(sf::Vector2f(25.f, 25.f));
	start.setFillColor(sf::Color(76,153,0));
	start.setPosition(40.f, 515.f);
	butoane.push_back(start);

	sf::RectangleShape ends (sf::Vector2f(170.0f, 50.0f));
	ends.setOrigin(sf::Vector2f(25.f, 25.f));
	ends.setFillColor(sf::Color(102,0,0));
	ends.setPosition(240.f, 515.f);
	butoane.push_back(ends);

	sf::RectangleShape input(sf::Vector2f(170.0f, 50.0f));
	input.setOrigin(sf::Vector2f(25.f, 25.f));
	input.setFillColor(sf::Color(255,128,0));
	input.setPosition(440.f, 515.f);
	butoane.push_back(input);

	sf::RectangleShape output (sf::Vector2f(170.0f, 50.0f));
	output.setOrigin(sf::Vector2f(25.f, 25.f));
	output.setFillColor(sf::Color(0,0,153));
	output.setPosition(40.f, 570.f);
	butoane.push_back(output);

	sf::RectangleShape atribuire(sf::Vector2f(170.0f, 50.0f));
	atribuire.setOrigin(sf::Vector2f(25.f, 25.f));
	atribuire.setFillColor(sf::Color(1,102,204));
	atribuire.setPosition(240.f, 570.f);
	butoane.push_back(atribuire);

	sf::RectangleShape comparatie (sf::Vector2f(170.0f, 50.0f));
	comparatie.setOrigin(sf::Vector2f(25.f, 25.f));
	comparatie.setFillColor(sf::Color(152,0,153));
	comparatie.setPosition(440.f, 570.f);
	butoane.push_back(comparatie);

}
//*2
void newBlock(int tip)
{
	blocStruct* temp = new blocStruct;
	if (tip > 1)
	{
		std::cout << "Blocul are expresia: ";
		std::cin >> temp->expresie_bloc_text;
	}
	if (tip == 5)
		temp->doua_posibilitati = 1;

	temp->bloc.setSize(sf::Vector2f(170.0f, 50.0f));
	temp->bloc.setPosition(250.f,250.f);
	temp->bloc.setOrigin(sf::Vector2f(25.f, 25.f));

    temp->culoare = tip;

	temp->text_bloc.setString(textBlocuri[tip]);
	temp->text_bloc.setCharacterSize(17);


	temp->expresie_bloc.setString(temp->expresie_bloc_text);
	temp->expresie_bloc.setFillColor(sf::Color::Black);
	temp->expresie_bloc.setCharacterSize(19);
	temp->expresie_bloc.setOrigin(sf::Vector2f(-25.f, 0.f));

	if (tip == 2)
		temp->text_bloc.setOrigin(sf::Vector2f(4.f, 0.f));
	if (tip == 3 or tip == 4)
		temp->text_bloc.setOrigin(sf::Vector2f(4.f, 0.f));
	if (tip == 5)
		temp->text_bloc.setOrigin(sf::Vector2f(4.f, 0.f));



	blocuri.push_back(temp);
}
//*2
int checkCoords(sf::RenderWindow& window, bool& buton)
{
	for (unsigned int i = 0; i < blocuri.size(); ++i)
		if (blocuri[i]->bloc.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
		{
			buton = 0;
			return i;
		}
	for (unsigned int i = 0; i < butoane.size(); ++i)
		if (butoane[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
		{
			buton = 1;
			return i;
		}
	if (meniu[5].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
	{
		StergereBlocuri();
	}
	if (meniu[8].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
	{
		ruleazaBlocuri();
	}

	return -1;
}
//desenez liniile dintre blocuri*1
void desenare_linie(sf::RenderWindow& window, sf::RectangleShape forma1, sf::RectangleShape forma2)
{

	sf::VertexArray linie(sf::LinesStrip, 2);
	linie[0].position = sf::Vector2f(forma1.getPosition().x+20, forma1.getPosition().y);
	linie[1].position = sf::Vector2f(forma2.getPosition().x+20, forma2.getPosition().y);
	window.draw(linie);
}

//fucntia care randeaza to ce este de desenat*1
void desenare_functie(sf::RenderWindow& window) {


	for (unsigned int i = 0; i < meniu.size(); ++i)
		window.draw(meniu[i]);

	for (unsigned int i = 0; i < butoane.size(); ++i)
		window.draw(butoane[i]);

	for (unsigned int i = 0; i < blocuri.size(); ++i)
	{
		if (blocuri[i]->urmatorul)
			desenare_linie(window, blocuri[i]->bloc, blocuri[i]->urmatorul->bloc);
		if (blocuri[i]->urmatorul2)
			desenare_linie(window, blocuri[i]->bloc, blocuri[i]->urmatorul2->bloc);
	}

	for (unsigned int i = 0; i < blocuri.size(); ++i) {
		blocuri[i]->bloc.setFillColor(culori[blocuri[i]->culoare]);
		window.draw(blocuri[i]->bloc);
	}

	for (unsigned int i = 0; i < texts.size(); ++i) {
		window.draw(texts[i]);
	}
}

//*2
void updateRoadsChange(sf::RenderWindow& window, int currentID, int anteriorID, int nrDrum)
{

	if (nrDrum == 0)
		blocuri[anteriorID]->urmatorul = blocuri[currentID];
	else
		blocuri[anteriorID]->urmatorul2 = blocuri[currentID];

	desenare_linie(window, blocuri[anteriorID]->bloc, blocuri[anteriorID]->urmatorul->bloc);
	if (blocuri[anteriorID]->urmatorul2)
		desenare_linie(window, blocuri[anteriorID]->bloc, blocuri[anteriorID]->urmatorul2->bloc);

}
//2
bool verificareExpresie(int tip, int currentID)
{
	std::string expresieN = expresieString.toAnsiString();
	if (tip == 2 or tip == 3)
	{
		if (expresieN.size() == 1 && expresieN[0] >= 65 && expresieN[0] <= 90)
		{
			blocuri[currentID]->culoare = tip;
			return 1;
		}
	}

	if (tip == 4)
	{
		if (expresieN[0] >= 65 && expresieN[0] <= 90 && expresieN[1] == '=' && expresieN[2] >= 65 && expresieN[2] <= 90 && (expresieN[3] == '+' or expresieN[3] == '-' or expresieN[3] == '*' or expresieN[3] == '/') && expresieN[4] >= 65 && expresieN[4] <= 90 && expresieN.size() == 5)
		{
			blocuri[currentID]->culoare = tip;
			return 1;
		}
		if (expresieN[0] >= 65 && expresieN[0] <= 90 && expresieN[1] == '=' && expresieN[2] >= 65 && expresieN[2] <= 90 && expresieN.size() == 3)
		{
			blocuri[currentID]->culoare = tip;
			return 1;
		}
	}

	if (tip == 5)
	{
		if (expresieN[0] >= 65 && expresieN[0] <= 90 && (expresieN[1] == '<' or expresieN[1] == '>') && expresieN[2] >= 65 && expresieN[2] <= 90 && expresieN.size() == 3)
		{
			blocuri[currentID]->culoare = tip;
			return 1;
		}
		if (expresieN[0] >= 65 && expresieN[0] <= 90 && expresieN[1] == '<' && expresieN[2] == '=' && expresieN[3] >= 65 && expresieN[3] <= 90 && expresieN.size() == 4)
		{
			blocuri[currentID]->culoare = tip;
			return 1;
		}
		if (expresieN[0] >= 65 && expresieN[0] <= 90 && expresieN[1] == '>' && expresieN[2] == '=' && expresieN[3] >= 65 && expresieN[3] <= 90 && expresieN.size() == 4)
		{
			blocuri[currentID]->culoare = tip;
			return 1;
		}
		if (expresieN[0] >= 65 && expresieN[0] <= 90 && expresieN[1] == '=' && expresieN[2] == '=' && expresieN[3] >= 65 && expresieN[3] <= 90 && expresieN.size() == 4)
		{
			blocuri[currentID]->culoare = tip;
			return 1;
		}
		if (expresieN[0] >= 65 && expresieN[0] <= 90 && expresieN[1] == '=' && expresieN[2] >= 65 && expresieN[2] <= 90 && expresieN.size() == 3)
		{
			blocuri[currentID]->culoare = tip;
			return 1;
		}
	}
	return 0;
}

//*3
int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "INTER SCHEME",sf::Style::Close | sf::Style::Titlebar);
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.setFramerateLimit(144);
    window.clear(sf::Color::White);
	sf::Font font;
	font.loadFromFile("times-new-roman.ttf");

	sf::Text expresieText;
	expresieText.setString(expresieString);
	expresieText.setFont(font);
	expresieText.setPosition(sf::Vector2f(603.f, 480.f));
	textEroare.setString("Expresie invalida");
	textEroare.setFont(font);
	textEroare.setFillColor(sf::Color::Red);
	textEroare.setPosition(sf::Vector2f(603.f, 515.f));
	textEroare.setCharacterSize(25.f);
	textCorect.setString("Expresie valida");
	textCorect.setFont(font);
	textCorect.setFillColor(sf::Color::Green);
	textCorect.setPosition(sf::Vector2f(603.f, 515.f));
	textCorect.setCharacterSize(25.f);

	{
		textBlocuri.push_back("INCEPUT");
		textBlocuri.push_back("SFARSIT");
		textBlocuri.push_back("INTRARE");
		textBlocuri.push_back("AFISARE");
		textBlocuri.push_back("ATRIBUIRE");
		textBlocuri.push_back("COMPARARE");
	}

	{
		culori.push_back(sf::Color(76,153,0));
		culori.push_back(sf::Color(102,0,0));
		culori.push_back(sf::Color(255,128,0));
		culori.push_back(sf::Color(0,0,153));
		culori.push_back(sf::Color(1,102,204));
		culori.push_back(sf::Color(152,0,153));
		culori.push_back(sf::Color(127, 127, 127));
	}

	{
		sf::Text clearButtonText;
		clearButtonText.setFont(font);
		clearButtonText.setPosition(628.f, 490.f);
		clearButtonText.setCharacterSize(40);
		clearButtonText.setString("STERGE");
		texts.push_back(clearButtonText);

		sf::Text startText;
		startText.setFont(font);
		startText.setPosition(40.f, 495.f);
		startText.setCharacterSize(30);
		startText.setString("INCEPUT");
		texts.push_back(startText);

		sf::Text endText;
		endText.setFont(font);
		endText.setPosition(245.f, 495.f);
		endText.setCharacterSize(30);
		endText.setString("SFARSIT");
		texts.push_back(endText);

		sf::Text inputText;
		inputText.setFont(font);
		inputText.setPosition(438.f, 495.f);
		inputText.setCharacterSize(30);
		inputText.setString("INTRARE");
		texts.push_back(inputText);

		sf::Text outputText;
		outputText.setFont(font);
		outputText.setPosition(40.f, 550.f);
		outputText.setCharacterSize(30);
		outputText.setString("AFISARE");
		texts.push_back(outputText);

		sf::Text assignText;
		assignText.setFont(font);
		assignText.setPosition(230.f, 550.f);
		assignText.setCharacterSize(30);
		assignText.setString("ATRIBUIE");
		texts.push_back(assignText);

		sf::Text compText;
		compText.setFont(font);
		compText.setPosition(430.f, 550.f);
		compText.setCharacterSize(29);
		compText.setString("COMPARA");
		texts.push_back(compText);

		sf::Text runText;
		runText.setFont(font);
		runText.setPosition(606.f, 545.f);
		runText.setCharacterSize(40);
		runText.setString("EXECUTA");
		texts.push_back(runText);

	}


	initMenu();
	initButoane();

	bool LeftPressed = false, RightPressed = false, draggable = false, buton = false;


	float mouseX = 0.f, mouseY = 0.f, startX = 0.f, startY = 0.f;

	int currentID = 0, anteriorID = 0, citireE = 0;

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonPressed:
				currentID = checkCoords(window, buton);
				if (event.mouseButton.button == sf::Mouse::Left && currentID > -1)
				{
					if (buton == 1)
					{
						if (currentID > 1) citireE = currentID + 1;
						if (limitStart == 1 && currentID == 0) break;
						if (limitEnd == 1 && currentID == 1) break;
						newBlock(currentID);
						if (currentID == 0)
						{
							limitStart = 1;
							startID = blocuri.size() - 1;
						}
						if (currentID == 1)
							limitEnd = 1;
						currentID = blocuri.size() - 1;
					}
					mouseX = blocuri[currentID]->bloc.getPosition().x;
					mouseY = blocuri[currentID]->bloc.getPosition().y;
					sf::Mouse::setPosition(sf::Vector2i(mouseX, mouseY), window);
					if (mouseX > 600) draggable = true;
					else draggable = false;
					LeftPressed = true;
				}
				else if (buton == 0 && event.mouseButton.button == sf::Mouse::Right && currentID > -1 && mouseX <= 600 && blocuri[currentID]->culoare != 1)
				{
					startX = blocuri[currentID]->bloc.getPosition().x;
					startY = blocuri[currentID]->bloc.getPosition().y;
					RightPressed = true;
				}
				break;

			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					LeftPressed = false;
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					if (RightPressed)
					{
						anteriorID = currentID;
						currentID = checkCoords(window, buton);
						if (buton == 0 && currentID > -1 && blocuri[currentID]->culoare != 0)
						{
							if (currentID == anteriorID)
							{
								blocuri[currentID]->urmatorul = NULL;
								blocuri[currentID]->urmatorul2 = NULL;
							}
							else
							{
								if (blocuri[currentID]->bloc.getPosition().x <= 575)
								{
									if (blocuri[currentID]->urmatorul == blocuri[anteriorID] && blocuri[currentID]->culoare != 5)
										blocuri[currentID]->urmatorul = NULL;
									if (blocuri[anteriorID]->culoare != 5)
										updateRoadsChange(window, currentID, anteriorID, 0);
									else
									{
										blocuri[anteriorID]->doua_posibilitati = (blocuri[anteriorID]->doua_posibilitati + 1) % 2;
										updateRoadsChange(window, currentID, anteriorID, blocuri[anteriorID]->doua_posibilitati);
									}
								}
							}
						}
					}
					RightPressed = false;
				}
			case sf::Event::MouseMoved:
				if (LeftPressed)
				{
					if (mouseX + 25 < 600) draggable = false;
					if (!draggable)
					{
						if (mouseX + 25 >= 600) sf::Mouse::setPosition(sf::Vector2i(575, sf::Mouse::getPosition(window).y), window);
						if (mouseX - 25 <= 3) sf::Mouse::setPosition(sf::Vector2i(28, sf::Mouse::getPosition(window).y), window);
						if (mouseY - 25 <= 3) sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition(window).x, 28), window);
						if (mouseY + 25 >= 600) sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition(window).x, 575), window);

					}
					mouseX = event.mouseMove.x;
					mouseY = event.mouseMove.y;
				}
				break;

			default:
				break;
			}
		}
		if (LeftPressed == true)
		{
			blocuri[currentID]->text_bloc.setFont(font);
			blocuri[currentID]->text_bloc.setPosition(mouseX - 10, mouseY - 18);
			blocuri[currentID]->expresie_bloc.setFont(font);
			blocuri[currentID]->expresie_bloc.setPosition(mouseX - 23, mouseY - 3);
			blocuri[currentID]->bloc.setPosition(mouseX, mouseY);
		}
		window.clear(sf::Color(0,51,51));
		desenare_functie(window);
		window.draw(expresieText);
		for (int i = 0u; i < blocuri.size(); ++i)
		{
			blocuri[i]->text_bloc.setFont(font);
			window.draw(blocuri[i]->text_bloc);
			blocuri[i]->expresie_bloc.setFont(font);
			window.draw(blocuri[i]->expresie_bloc);
		}
		window.display();
	}
	window.close();
	return 0;
}
