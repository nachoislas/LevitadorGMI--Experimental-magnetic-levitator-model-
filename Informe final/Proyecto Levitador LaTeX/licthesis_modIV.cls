%% licthesis Class for LaTeX2e.


% esta es un estilo para escribir tesis de doctorado en el
% Laboratorio de Instrumentaci\'{o}n y Control de la
% Universidad Nacional de Mar del Plata, Argentina

% esta basado en el estilo de University of Manchester, UK





%%
%% Time-stamp: <July 19, 1999 10:42 graham@cs.man.ac.uk>
%%

%%
%% Copyright 1999 G.D. Gough and the University of Manchester
%%
%% This program can redistributed and/or modified under the terms
%% of the LaTeX Project Public License Distributed from CTAN
%% archives in directory macros/latex/base/lppl.txt; either
%% version 1 of the License, or (at your option) any later version.

%% This class file has been cobbled together for use of postgraduate
%% students in the Department of Computer Science at the University of
%% Manchester. I believe it had its origins in the Stanford thesis
%% style but has since been modified almost beyond recognition to cope
%% with life in Manchester.

%% A fairly detailed description of the use of this class can be found
%% below (see `Example of use')

%% Any comments or problems about its use should be made
%% via the local newsgroup man.cs.tex.
%%
%% The \dept command can be used to define a department other than
%% Computer Science,
%% e.g \dept{Applied Basket Weaving}
%% but no support is provided for users outside the Department of C.Sc.
%%
%% Note that this makes no attempt to deal with multi-volume thesis -
%% if you do one that long you deserve everything you get.
%% 1) Page numbering starts on the title page and is arabic (i.e. title
%%       page is no.1), all pages are numbered (declaration, figures
%%       etc included), but the title page itself shall not show the
%%       page number!
%%
%%
%% 2) The preface will consist of the following sections in the
%%       following order!
%%       i  )    Title Page
%%       ii )    Contents page (actually we shall be fancy and have a
%%               table of contents, list of tables and list of figures.
%%               This is the correct place to put these extras)
%%       iii)    A short Abstract
%%       iv)     Declaration of honesty
%%       v)      Statement of qualifications and research
%%       vi)     Anything else you want, such as acknowledgements
%%
%%
%% 3) The ordering of the items on the title page has been changed
%%       slightly from the old muthesis style - it is now
%%               Statement - A thesis ..... Faculty of ...
%%               Year of Presentation
%%               Candidate's name
%%               Candidate's department
%%
%%
%% 4)    Certain members of this university never having heard of
%%       typesetting, (i.e. the ones who will be the first against the
%%       wall when the revolution comes), the whole thing (apart from
%%       footnotes and quotations must be one and a half spaced. By a
%%       strange coincidence, a set of the thesis rules fell through a
%%       time warp from the future and landed on my desk. These note
%%       that single spacing is allowed in typeset thesis, with a
%%       footnote that those who were in favour of double spacing were
%%       the first against the wall when the revolution came.
%%               If everyone who submits a thesis goes and argues with
%%       them about this they may eventually get the hint, but this is
%%       unlikely. By all means use the singlespace class option while
%%       producing draft copies, but remember to remove it before you
%%       hand in.
%%
%%
%% 5) The binding (left hand) margin must be no less than 40mm, other
%%       margins must be no less than 15mm, with page numbers falling
%%       within these margins. The binding margin is thus rather large,
%%       even when you take the binding (normally about 10mm) into account.
%%       Consequently you cannot reduce the other margins without
%%       making the page look unbalanced. Consequently we have a 40mm
%%       binding margin and 25 mm margin on all other sides. NOTE, this
%%       does not allow for any marginal notes. I recommend that you
%%       avoid using these, since a) there is not much space for them
%%       and they will format badly and b) they will violate the margin
%%       rules.
%%
%%
%% The rest of these notes will not be found in the written submission
%% rules, but come from conversations with the exams office.
%%
%%
%% 6)    They have not particular rules about the format of the
%%       bibliography, so long as you are consistent in your naming
%%       scheme. I would recommend the alpha style.
%%
%%
%% 7)    They do not seem to have any rules about appendices.
%%
%%
%% 8)    As before, any extra things you want to put in -
%%       acknowledgements etc, should come after the mandatory preface
%%       sections and before the first chapter.
%%
%%
%% It is not compatible with the drafthead style.


\NeedsTeXFormat{LaTeX2e} \ProvidesClass{licthesis_modIV}[2013/01/22 LIC Thesis Class]

\RequirePackage{setspace}
\RequirePackage{fancyhdr}
\newif\ifsinglesp@
\newif\ifdoublesp@
\doublesp@true
\newif\ifmyd@

\DeclareOption{mydraft}{\myd@true}
\DeclareOption{singlespace}{\singlesp@true}
\DeclareOption{doublespace}{\doublesp@true}
\DeclareOption{Doctor}{\def\@degreetitle{Doctor en Ingenier\'{\i}a orientaci\'on Electr\'onica}\def\@tipoinforme{Trabajo de Tesis}}
\DeclareOption{Maestria}{\def\@degreetitle{Mag\'{\i}ster}\def\@tipoinforme{Trabajo de Tesis}}
\DeclareOption{Grado}{\def\@degreetitle{Ingeniero en Electr\'{o}nica}\def\@tipoinforme{Trabajo Final de carrera}}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}

\ProcessOptions

\LoadClass[a4paper]{report}

%% LIC thesis style -- modifications to the report style
%%
\long\def\comment#1{}
\comment{

  Example of use:
    \documentclass[12pt,MSc]{muthesis}
%%%% \documentclass[12pt,MSc,singlespace]{muthesis}
%%%%  can be used until the final draft
    \begin{document}
    \title{How to Write Theses\\
            With Two Line Titles}
    \author{John Henry Candidate}
    \principaladviser{John Parker}
    \firstreader{John Green}
    \secondreader{John BigBooty\\(Another Department)}

    \beforeabstract
    \prefacesection{Abstract}
        This thesis answers all the questions you were afraid to ask ...
    \afterabstract
    \prefacesection{Acknowledgements}
        I would like to thank...
    \afterpreface

    \chapter{Introduction}
         ...
    \chapter{Conclusions}
         ...
    \appendix
    \chapter{A Long Proof}
         ...
    \bibliographystyle{alpha}
    \bibliography{mybib}


Documentation:
    It sets the margins and interline spacing and disallows page
    breaks at hyphens.

    The \beforeabstract command creates the title page, a copyright
    page (default), and a signature page (optional), the tables of
    contents, tables and figures. The signature page is not mentioned
    in the rules for submission, and by implication are not allowed.

    The user then includes the preface using the
    \prefacesection{Abstract} command.

    Next should come an \afterabstract command to insert the declaration.

    Then the user should put preface section(s), using the
    \prefacesection{section title} command. The first of these should
    be the statement of education and research, others may be
    acknowledgements etc.

    The \afterpreface command then  sets things up to start
    the main body (continuing the sequence of page numbering, but
    putting page headings etc.).

    Having one of \beforepreface or \afterpreface without the other
    may cause problems, so udon't do it.

    The following commands can control what goes in the front matter
    material:

        \title{thesis title}
        \author{author's name}
        \dept{author's department}
                - Computer Science if omitted
        \principaladviser{the principal advisor's name}
           (or \principaladvisor, if you prefer advisor spelled with o)
        \firstreader{the first reader's name}
        \secondreader{the second reader's name}
        \submitdate{month year in which submitted to GPO}
                - date LaTeX'd if omitted
        \copyrightyear{year degree conferred (next year if submitted in Dec.)}
                - year LaTeX'd (or next year, in December) if omitted
        \copyrighttrue or \copyrightfalse
                - produce or don't produce a copyright page (true by default)
        \figurespagetrue or \figurespagefalse
                - produce or don't produce a List of Figures page
                  (true by default)
        \tablespagetrue or \tablespagefalse
                - produce or don't produce a List of Tables page
                  (true by default)
        \sigpagtrue or \sigpagfalse
                - produce or don't produce a signature page
                  (again false by default, as Manchester theses don't
                   require this. However, it's left in so that you can
                   your own `examiners page' if you like!)

        If you set all these to false, then perhaps you don't want any
        preface at all, so perhaps you hould leave out both \beforepreface
        and \afterpreface -).

        This style uses interline spacing that is 1.24 times normal
        (about one and half spaced), except in the figure and table
        environments where normal spacing is used.

        The example above shows the 12pt substyle being used.  This
        seems to give acceptable looking results, but it may be
        omitted to get smaller print.

}
%% We need >15mm  margins except on the binding edge, where it is >40mm
\oddsidemargin 14.6truemm \evensidemargin 0truemm
\marginparwidth 40pt \marginparsep 10pt
\topmargin 0truemm \headsep 10truemm
\textheight 224truemm \textwidth 145truemm

%% Disallow page breaks at hyphens (this will give some underfull vbox's,
%% so an alternative is to use \brokenpenalty=100 and manually search
%% for and fix such page breaks)
\brokenpenalty=10000

%% Set up line spacing
\AtBeginDocument{%
\ifdoublesp@%
  \doublespacing%
\else\ifsinglesp@%
  \singlespacing%
\else%
  \onehalfspacing%
\fi%
\fi
}

%% \author, \title are defined in report; here are the rest of the
%% front matter defining macros
%% Note: If you really want, you can define your own macro for, say
%% \supervisor , just by copying the definitions of \principaladvisor
%%
\def\departamento#1{\gdef\@dept{#1}}

\def\director#1{\gdef\@director{#1}}
\def\dedication#1{\gdef\@dedication{#1}}
\def\affiliation#1{\gdef\@affiliation{#1}}

\def\authoreng#1{\gdef\@authoreng{#1}}

\newif\ifnocodirector
\def\codirector#1{\gdef\@codirector{#1}\nocodirectortrue}
\def\@codirector{}

\def\@director{}\def\@dept{}

\def\thesisdate#1{\gdef\@thesisdate{#1}}
\def\thesisdateeng#1{\gdef\@thesisdateeng{#1}}

\def\submitdate#1{\gdef\@submitdate{#1}}
\def\copyrightyear#1{\gdef\@copyrightyear{#1}} % \author, \title in report
\def\@title{}\def\@author{}\def\@dept{Departamento de Electr\'onica}
\def\@submitdate{\ifcase\the\month\or
  Enero\or Febrero\or Marzo\or Abril\or Mayo\or Junio\or
  Julio\or Agosto\or Septiembere\or Octubre\or Noviembre\or Diciembre\fi
  \space \number\the\year}

\newif\iffigurespage \newif\iftablespage
 \figurespagetrue \tablespagetrue

\def\shorttitle#1{\gdef\@shorttitle{#1}}

\def\titlep{%
        %\pagestyle{empty}
        \thispagestyle{empty}%
        \null\vskip1in%
        \begin{center}
                \Huge\MakeUppercase{\expandafter{\@title}}    % ac\'{a} reparamos el BUG de uppercase

                \vfill

                \Large \@author\\
        \end{center}
        \vfill


        \begin{center}
        Este \@tipoinforme~ fue presentado al \@dept \
        de la Facultad de Ingenier\'{\i}a de la Universidad Nacional de\\ Mar del Plata el \@thesisdate, como requisito parcial para\\ la obtenci\'{o}n del t\'{\i}tulo de \\
        \large{\@degreetitle}

        \vfill

        Director: \@director\\

        % insertar un condicional para el caso  que no exista el codirector

        \ifnocodirector{Co-Director: \@codirector\\}\fi

\end{center}\vskip.5in\newpage}

\def\affiliationp{%
        \thispagestyle{empty}%
        \null\vskip1in%
        \begin{center}
                \@affiliation
        \end{center}
\vskip.5in\newpage}

\def\dedicationp{%
        \thispagestyle{empty}%
        \null\vskip1in%
        \begin{flushright}
                \@dedication
        \end{flushright}
\vskip.5in\newpage}




%-------------------------------------------------------------------------------------------------------------
\def\englishtitle#1{\gdef\@englishtitle{#1}} \def\@englishtitle{}
\def\englishaffiliation#1{\gdef\@englishaffiliation{#1}} \def\@englishaffiliation{}
\def\englishcover{
        \pagestyle{empty}
        \thispagestyle{empty}%
        \null\vskip1in%
        \begin{center}
                \Huge\MakeUppercase{\expandafter{\@englishtitle}}    % ac\'{a} reparamos el BUG de uppercase

                \vfill

                \Large \@authoreng\\
        \end{center}
        \vfill


        \begin{center}
		This Thesis work was submitted to the Electronics Department, \\
		School of Engineering, National University of Mar del Plata \\
		on \@thesisdateeng~in partial fulfillment of the requirements for the degree of\\
        \large{PhD in Electronics Engineering}

        \vfill
		
		\begin{center}
			Thesis supervisors: \\
			\@director~
			% insertar un condicional para el caso  que no exista el codirector
			\ifnocodirector{and~\@codirector\\}\fi
		\end{center}
		
\end{center}\vskip.5in\newpage
        \thispagestyle{empty}%
		\pagestyle{empty}%
        \null\vskip1in%
        \begin{center}
                \@englishaffiliation
        \end{center}
\vskip.5in\newpage

        }

\def\beforeabstract{
        \pagenumbering{roman}
        \pagestyle{plain}
        \titlep
      %  \affiliationp
      %\dedicationp
        \tableofcontents
        \newpage
        \iftablespage
                \addvspace{10pt}
                \listoftables
                \newpage
        \fi
        \iffigurespage
                \addvspace{10pt}
                \listoffigures
                \newpage
        \fi}

\def\afterabstract{}
% %% \newpage inserted by DPC 15/9/93
% %% make the declaration come on the page after the abstract.
%         \newpage
%         \declpage
%         \ifcopyright\copyrightpage\fi % moved to conform with 95 regs
%         }
%
% %% The declaration page above is the standard
% %% declaration, but if you want the other sort ( where you admit
% %% that parts of your thesis have been used elsewhere) then you'll
% %% have to hack it yourself -- just replace the text by what you'd like
% %%
\def\prefacesection#1{%
        \chapter*{#1}
        \addcontentsline{toc}{chapter}{#1}}

%% The \addvspace business below is to avoid the extra space that gets put
%% in the tables of figures and tables between chapters (it makes no sense
%% now that the numbering is continuous)
\def\afterpreface{\newpage
        \pagestyle{headings}
        \pagenumbering{arabic}
        \pagestyle{fancy}
        \fancyhf{} %delete the current section for header and footer
        \fancyhead[LE,RO]{\footnotesize\bfseries\thepage}
        \fancyhead[LO]{\footnotesize \nouppercase\leftmark}
        \fancyhead[RE]{\footnotesize\nouppercase\@shorttitle}
        }



\ifmyd@
    \usepackage[center]{background}
    \SetBgContents{}
    \SetBgHshift{0}
    \SetBgVshift{250}
    \SetBgColor{black!100}
    \SetBgScale{0.8}
    \SetBgOpacity{1}
    \AddEverypageHook{%
    \ifthenelse{\isodd{\value{page}}}
        {\SetBgContents{\@title \ --- \ \@author \ --- \ \emph{Draft: \today}}%
         \SetBgAngle{90}}%
        {\SetBgContents{\@title \ --- \ \@author \ --- \ \emph{Draft: \today}}%
         \SetBgAngle{-90}}%
        {\bg@material}%
        }
    \fi



%% This produces the chapter title at the top of each page. It can be
%% changed in the obvious way, e.g., \pagestyle{empty} or \pagestyle{plain}
%%

%%% DPC 15/9/93 Make the bibliography generate an entry in the table
%%% of contents.

\def\thebibliography#1{\singlespacing\def\refname{Bibliograf\'{\i}a}\chapter*{
\addtocounter{chapter}{1}\refname\@mkboth
  {\refname}{\thechapter\hspace{1ex}\refname}}%
  \addcontentsline{toc}{chapter}{\refname}%
  \list
  {\@biblabel{\arabic{enumiv}}}{\settowidth\labelwidth{\@biblabel{#1}}%
    \leftmargin\labelwidth
    \advance\leftmargin\labelsep
    \usecounter{enumiv}%
    \let\p@enumiv\@empty
    \def\theenumiv{\arabic{enumiv}}}%
    \def\newblock{\hskip .11em plus.33em minus.07em}%
    \sloppy\clubpenalty4000\widowpenalty4000
    \sfcode`\.=\@m}

\def\endthebibliography{%
  \def\@noitemerr{\@warning{La definicion del ambiente `thebibliography' no fue encontrada. Este ambiente debe estar.}}%
  \endlist\onehalfspacing}
