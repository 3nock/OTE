# Contributing

Welcome to Sub3 Suite! This document is relevant for you if you want to contribute to the project.

## Overview

This is a list of things you can do to help out (in no particular order). Each item will be expanded upon later in the document.

- [Compile sub3suite](COMPILING.md) and add new features and/or solving current [issues](https://github.com/3nock/sub3suite/issues).
- Design an even more intuitive UI, Logos and Icons.
- Report [bugs](https://github.com/3nock/sub3suite/issues) at the issue tracker.
- Add feature requests to the [issue tracker](https://github.com/3nock/sub3suite/issues).
- Write a blog post about sub3suite in any blog and you can inform us of the post so we can share it.
- [Contact us](CONTACTS.md) and talk about sub3suite.
- Send a [donation](SPONSOR.md) to support the project.
- Help us improve the [documentation](https://github.com/3nock/s3s_doc).

### Compile sub3suite

There is a guide to [compiling the whole project](COMPILING.md) available. Since sub3suite is still in its early development stage its easy compile the project, not much is required.

Compiling sub3suite is very useful to us (even if you don't add any code). Your experience can improve this guide and help new contributors.

### Getting started with development

As with any open source project, documentation is lacking and the code can seem very daunting at first. A list of resources that can help you understand the architecture and get you started will be posted soon.

The feedback of new contributors is vital.

### Sending a pull request

Here is a little guide on how to do a clean pull request for people who don't yet know how to use git. We recommend using [Git Extensions](https://gitextensions.github.io), but any git interface is fine.

1. First we need to [fork](https://help.github.com/articles/fork-a-repo/) the actual sub3suite repo on our github account.
2. When the fork is finished, clone the repo `git clone https://github.com/myname/sub3suite.git`.
3. When pushing new features/bug/whatever to a github project the best practice is to create branches. The command `git checkout -b my-branch-name` will automatically create a branch and check it out.
4. Make all the changes you want and when finishing it, use `git add myfiles` to add it to the repo.
5. Commit your change. `git commit -m 'a message about what you changed'`. The change are applied to your local git repo.
6. Push it to your `origin`. The `origin` is your repo which is hosted on github. `git push --set-upstream origin your-branch-name`.
7. Sync with the `upstream` repo, the real sub3suite repo. `git remote add upstream https://github.com/3nock/sub3suite.git`, using `git remote -v` will show which origin/upstream are setup in the local repo.
8. Sync your fork with the `upstream`, `git fetch upstream`. Now checkout your local `development` branch again `git checkout development` and merge the upstream `git merge upstream/development`.
9. Time to create the pull request! Using the github ui, go to your account/repo, select the branch you already pushed, and click `Pull request`. Review your pull request and send it.

### Report bugs

If you want to have the highest chance of getting your problem solved, you are going to have to put in some effort. The vital things are:

1. Search the issue tracker to see if your bug has not been reported already.
2. Give concrete steps on how to reproduce your bug.
3. Tell us exactly which version of sub3suite you used and the environment(s) you reproduced the bug in.

You can take a look at the [issue template](https://github.com/3nock/sub3suite/blob/development/.github/ISSUE_TEMPLATE.md) for more details.

### Request features

If you request a Feature, make sure to give a clear description of the desired behaviour and give clear examples of cases where your feature would be useful.

We understand that it can be disappointing to not get your feature implemented, but opening an issue is the best way to communicate it regardless.

### Write a blogpost

Sub3Suite has no official blog yet but regardless of what blogging platform you use we welcome and encourage anyone to write blog posts about su3suite at any complexicity levels. 
Please [contact us](CONTACTS.md) and inform us about your post and we will try our best to share the blog post with all sub3suite community members.

### Contact us

There are several ways to reach out to the community of sub3suite developers, contributors and users. [Telegram](https://telegram.me/sub3suite) & [Twitter](http://twitter.com/sub3suite). Most questions regarding contributing, developing and using sub3suite can be answered there.

### Credits

This `CONTRIBUTING.md` File is a our simplified version of the [x64dbg's](https://github.com/x64dbg/x64dbg) `CONTRIBUTING.md` File :)
