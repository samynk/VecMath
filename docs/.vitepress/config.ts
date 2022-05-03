import { defineConfig } from "vitepress";


function GetIntroductionSideBar() {
    return [
        { 
            text: "Introduction", collapsable: true,
            children:[
                {text: 'Objectives', link: '/introduction/objectives'},
                {text: 'Overview', link: '/introduction/overview'},
            ] 
        },
        {
            text: 'Language', collapsable: true,
            children: [
                {text: 'Variables', link: '/introduction/variables'},
                {text: 'Magnitude', link: '/introduction/operators/magnitude'},
                {text: 'Multiply', link: '/introduction/operators/multiply'},
                {text: 'Division', link: '/introduction/operators/division'},
                {text: 'DotProduct', link: '/introduction/operators/dotproduct'},
                {text: 'Power', link: '/introduction/operators/power'},
                {text: 'Add', link: '/introduction/operators/add'},
                {text: 'Subtract', link: '/introduction/operators/subtract'},
                {text: 'Functions', link: '/introduction/functions/functions'}
            ]
        },
        {
            text: 'Commands', collapsable: true, link: '/introduction/commands/commands'
        }
    ]
}

export default defineConfig({
    themeConfig: {
        nav: [
            { text: 'Github', link: 'https://github.com/samynk/VecMath' },
            { text: 'Download', link: 'https://github.com/samynk/VecMath/releases'}
        ],
        sidebar: {
            '/introduction/': GetIntroductionSideBar(),
            '/': GetIntroductionSideBar()
        },
    },
    markdown: {
        lineNumbers: true
    },
    lastUpdated: true
});

